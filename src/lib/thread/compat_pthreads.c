/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file compat_pthreads.c
 *
 * \brief Implementation for the pthreads-based multithreading backend
 * functions.
 */

#include "orconfig.h"
#include "lib/thread/threads.h"
#include "lib/wallclock/timeval.h"
#include "lib/log/log.h"
#include "lib/log/util_bug.h"

#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>

/** Wraps a void (*)(void*) function and its argument so we can
 * invoke them in a way pthreads would expect.
 */
typedef struct qed_hs_pthread_data_t {
  void (*func)(void *);
  void *data;
} qed_hs_pthread_data_t;
/** Given a qed_hs_pthread_data_t <b>_data</b>, call _data-&gt;func(d-&gt;data)
 * and free _data.  Used to make sure we can call functions the way pthread
 * expects. */
static void *
qed_hs_pthread_helper_fn(void *_data)
{
  qed_hs_pthread_data_t *data = _data;
  void (*func)(void*);
  void *arg;
  /* mask signals to worker threads to avoid SIGPIPE, etc */
  sigset_t sigs;
  /* We're in a subthread; don't handle any signals here. */
  sigfillset(&sigs);
  pthread_sigmask(SIG_SETMASK, &sigs, NULL);

  func = data->func;
  arg = data->data;
  qed_hs_free(_data);
  func(arg);
  return NULL;
}
/**
 * A pthread attribute to make threads start detached.
 */
static pthread_attr_t attr_detached;
/** True iff we've called qed_hs_threads_init() */
static int threads_initialized = 0;

/** Minimalist interface to run a void function in the background.  On
 * Unix calls pthread_create, on win32 calls beginthread.  Returns -1 on
 * failure.
 * func should not return, but rather should call spawn_exit.
 *
 * NOTE: if <b>data</b> is used, it should not be allocated on the stack,
 * since in a multithreaded environment, there is no way to be sure that
 * the caller's stack will still be around when the called function is
 * running.
 */
int
spawn_func(void (*func)(void *), void *data)
{
  pthread_t thread;
  qed_hs_pthread_data_t *d;
  if (PREDICT_UNLIKELY(!threads_initialized)) {
    qed_hs_threads_init();
  }
  d = qed_hs_malloc(sizeof(qed_hs_pthread_data_t));
  d->data = data;
  d->func = func;
  if (pthread_create(&thread, &attr_detached, qed_hs_pthread_helper_fn, d)) {
    qed_hs_free(d);
    return -1;
  }

  return 0;
}

/** End the current thread/process.
 */
void
spawn_exit(void)
{
  pthread_exit(NULL);
}

/** Return an integer representing this thread. */
unsigned long
qed_hs_get_thread_id(void)
{
  union {
    pthread_t thr;
    unsigned long id;
  } r;
  r.thr = pthread_self();
  return r.id;
}

/* Conditions. */

/** Initialize an already-allocated condition variable. */
int
qed_hs_cond_init(qed_hs_cond_t *cond)
{
  pthread_condattr_t condattr;

  memset(cond, 0, sizeof(qed_hs_cond_t));
  /* Default condition attribute. Might be used if clock monotonic is
   * available else this won't affect anything. */
  if (pthread_condattr_init(&condattr)) {
    return -1;
  }

#if defined(HAVE_CLOCK_GETTIME)
#if defined(HAVE_PTHREAD_CONDATTR_SETCLOCK) && \
  defined(CLOCK_MONOTONIC)
  /* Use monotonic time so when we timedwait() on it, any clock adjustment
   * won't affect the timeout value. */
  if (pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC)) {
    return -1;
  }
#define USE_COND_CLOCK CLOCK_MONOTONIC
#else /* !(defined(HAVE_PTHREAD_CONDATTR_SETCLOCK) && ...) */
  /* On OSX Sierra, there is no pthread_condattr_setclock, so we are stuck
   * with the realtime clock.
   */
#define USE_COND_CLOCK CLOCK_REALTIME
#endif /* defined(HAVE_PTHREAD_CONDATTR_SETCLOCK) && ... */
#endif /* defined(HAVE_CLOCK_GETTIME) */
  if (pthread_cond_init(&cond->cond, &condattr)) {
    return -1;
  }
  return 0;
}

/** Release all resources held by <b>cond</b>, but do not free <b>cond</b>
 * itself. */
void
qed_hs_cond_uninit(qed_hs_cond_t *cond)
{
  if (pthread_cond_destroy(&cond->cond)) {
    // LCOV_EXCL_START
    log_warn(LD_GENERAL,"Error freeing condition: %s", strerror(errno));
    return;
    // LCOV_EXCL_STOP
  }
}
/** Wait until one of the qed_hs_cond_signal functions is called on <b>cond</b>.
 * (If <b>tv</b> is set, and that amount of time passes with no signal to
 * <b>cond</b>, return anyway.  All waiters on the condition must wait holding
 * the same <b>mutex</b>.  All signallers should hold that mutex.  The mutex
 * needs to have been allocated with qed_hs_mutex_init_for_cond().
 *
 * Returns 0 on success, -1 on failure, 1 on timeout. */
int
qed_hs_cond_wait(qed_hs_cond_t *cond, qed_hs_mutex_t *mutex, const struct timeval *tv)
{
  int r;
  if (tv == NULL) {
    while (1) {
      r = pthread_cond_wait(&cond->cond, &mutex->mutex);
      if (r == EINTR) {
        /* EINTR should be impossible according to POSIX, but POSIX, like the
         * Pirate's Code, is apparently treated "more like what you'd call
         * guidelines than actual rules." */
        continue; // LCOV_EXCL_LINE
      }
      return r ? -1 : 0;
    }
  } else {
    struct timeval tvnow, tvsum;
    struct timespec ts;
    while (1) {
#if defined(HAVE_CLOCK_GETTIME) && defined(USE_COND_CLOCK)
      if (clock_gettime(USE_COND_CLOCK, &ts) < 0) {
        return -1;
      }
      tvnow.tv_sec = ts.tv_sec;
      tvnow.tv_usec = (int)(ts.tv_nsec / 1000);
      timeradd(tv, &tvnow, &tvsum);
#else /* !(defined(HAVE_CLOCK_GETTIME) && defined(USE_COND_CLOCK)) */
      if (gettimeofday(&tvnow, NULL) < 0)
        return -1;
      timeradd(tv, &tvnow, &tvsum);
#endif /* defined(HAVE_CLOCK_GETTIME) && defined(USE_COND_CLOCK) */

      ts.tv_sec = tvsum.tv_sec;
      ts.tv_nsec = tvsum.tv_usec * 1000;

      r = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &ts);
      if (r == 0)
        return 0;
      else if (r == ETIMEDOUT)
        return 1;
      else if (r == EINTR)
        continue;
      else
        return -1;
    }
  }
}
/** Wake up one of the waiters on <b>cond</b>. */
void
qed_hs_cond_signal_one(qed_hs_cond_t *cond)
{
  pthread_cond_signal(&cond->cond);
}
/** Wake up all of the waiters on <b>cond</b>. */
void
qed_hs_cond_signal_all(qed_hs_cond_t *cond)
{
  pthread_cond_broadcast(&cond->cond);
}

int
qed_hs_threadlocal_init(qed_hs_threadlocal_t *threadlocal)
{
  int err = pthread_key_create(&threadlocal->key, NULL);
  return err ? -1 : 0;
}

void
qed_hs_threadlocal_destroy(qed_hs_threadlocal_t *threadlocal)
{
  pthread_key_delete(threadlocal->key);
  memset(threadlocal, 0, sizeof(qed_hs_threadlocal_t));
}

void *
qed_hs_threadlocal_get(qed_hs_threadlocal_t *threadlocal)
{
  return pthread_getspecific(threadlocal->key);
}

void
qed_hs_threadlocal_set(qed_hs_threadlocal_t *threadlocal, void *value)
{
  int err = pthread_setspecific(threadlocal->key, value);
  qed_hs_assert(err == 0);
}

/** Set up common structures for use by threading. */
void
qed_hs_threads_init(void)
{
  if (!threads_initialized) {
    qed_hs_locking_init();
    const int ret1 = pthread_attr_init(&attr_detached);
    qed_hs_assert(ret1 == 0);
#ifndef PTHREAD_CREATE_DETACHED
#define PTHREAD_CREATE_DETACHED 1
#endif
    const int ret2 =
      pthread_attr_setdetachstate(&attr_detached, PTHREAD_CREATE_DETACHED);
    qed_hs_assert(ret2 == 0);
    threads_initialized = 1;
  }
  set_main_thread();
}
