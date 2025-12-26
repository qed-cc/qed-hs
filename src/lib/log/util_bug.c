/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file util_bug.c
 **/

#include "orconfig.h"
#include "lib/log/util_bug.h"
#include "lib/log/log.h"
#include "lib/err/backtrace.h"
#include "lib/err/torerr.h"
#ifdef QED_HS_UNIT_TESTS
#include "lib/smartlist_core/smartlist_core.h"
#include "lib/smartlist_core/smartlist_foreach.h"
#endif
#include "lib/malloc/malloc.h"
#include "lib/string/printf.h"
#include "lib/thread/threads.h"

#include <string.h>
#include <stdlib.h>

#ifdef QED_HS_UNIT_TESTS
static void (*failed_assertion_cb)(void) = NULL;
static int n_bugs_to_capture = 0;
static smartlist_t *bug_messages = NULL;
#define capturing_bugs() (bug_messages != NULL && n_bugs_to_capture)
void
qed_hs_capture_bugs_(int n)
{
  qed_hs_end_capture_bugs_();
  bug_messages = smartlist_new();
  n_bugs_to_capture = n;
}
void
qed_hs_end_capture_bugs_(void)
{
  n_bugs_to_capture = 0;
  if (!bug_messages)
    return;
  SMARTLIST_FOREACH(bug_messages, char *, cp, qed_hs_free(cp));
  smartlist_free(bug_messages);
  bug_messages = NULL;
}
const smartlist_t *
qed_hs_get_captured_bug_log_(void)
{
  return bug_messages;
}
static void
add_captured_bug(const char *s)
{
  --n_bugs_to_capture;
  smartlist_add_strdup(bug_messages, s);
}
/** Set a callback to be invoked when we get any qed_hs_bug_occurred_
 * invocation. We use this in the unit tests so that a nonfatal
 * assertion failure can also count as a test failure.
 */
void
qed_hs_set_failed_assertion_callback(void (*fn)(void))
{
  failed_assertion_cb = fn;
}
#else /* !defined(QED_HS_UNIT_TESTS) */
#define capturing_bugs() (0)
#define add_captured_bug(s) do { } while (0)
#endif /* defined(QED_HS_UNIT_TESTS) */

/** Helper for qed_hs_assert: report the assertion failure. */
void
qed_hs_assertion_failed_(const char *fname, unsigned int line,
                      const char *func, const char *expr,
                      const char *fmt, ...)
{
  char *buf = NULL;
  char *extra = NULL;
  va_list ap;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  if (fmt) {
    va_start(ap,fmt);
    qed_hs_vasprintf(&extra, fmt, ap);
    va_end(ap);
  }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  log_err(LD_BUG, "%s:%u: %s: Assertion %s failed; aborting.",
          fname, line, func, expr);
  qed_hs_asprintf(&buf, "Assertion %s failed in %s at %s:%u: %s",
               expr, func, fname, line, extra ? extra : "");
  qed_hs_free(extra);
  log_backtrace(LOG_ERR, LD_BUG, buf);
  qed_hs_free(buf);
}

static atomic_counter_t total_bug_reached;

void
qed_hs_bug_init_counter(void)
{
  atomic_counter_init(&total_bug_reached);
}

/** Helper to update BUG count in metrics. */
void
qed_hs_bug_increment_count_(void)
{
  atomic_counter_add(&total_bug_reached, 1);
}

size_t
qed_hs_bug_get_count(void)
{
  return atomic_counter_get(&total_bug_reached);
}

/** Helper for qed_hs_assert_nonfatal: report the assertion failure. */
void
qed_hs_bug_occurred_(const char *fname, unsigned int line,
                  const char *func, const char *expr,
                  int once, const char *fmt, ...)
{
  char *buf = NULL;
  const char *once_str = once ?
    " (Future instances of this warning will be silenced.)": "";
  if (! once) {
    // _once assertions count from the macro directly so we count them as many
    // time as they are reached, and not just once.
    qed_hs_bug_increment_count_();
  }
  if (! expr) {
    if (capturing_bugs()) {
      add_captured_bug("This line should not have been reached.");
      return;
    }
    log_warn(LD_BUG, "%s:%u: %s: This line should not have been reached.%s",
             fname, line, func, once_str);
    qed_hs_asprintf(&buf,
                 "Line unexpectedly reached at %s at %s:%u",
                 func, fname, line);
  } else {
    if (capturing_bugs()) {
      add_captured_bug(expr);
      return;
    }

    va_list ap;
    char *extra = NULL;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
    if (fmt) {
      va_start(ap,fmt);
      qed_hs_vasprintf(&extra, fmt, ap);
      va_end(ap);
    }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    log_warn(LD_BUG, "%s:%u: %s: Non-fatal assertion %s failed.%s",
             fname, line, func, expr, once_str);
    qed_hs_asprintf(&buf, "Non-fatal assertion %s failed in %s at %s:%u%s%s",
                 expr, func, fname, line, fmt ? " : " : "",
                 extra ? extra : "");
    qed_hs_free(extra);
  }
  log_backtrace(LOG_WARN, LD_BUG, buf);
  qed_hs_free(buf);

#ifdef QED_HS_UNIT_TESTS
  if (failed_assertion_cb) {
    failed_assertion_cb();
  }
#endif
}

/**
 * Call the qed_hs_raw_abort_() function to close raw logs, then kill the current
 * process with a fatal error. But first, close the file-based log file
 * descriptors, so error messages are written before process termination.
 *
 * (This is a separate function so that we declare it in util_bug.h without
 * including torerr.h in all the users of util_bug.h)
 **/
void
qed_hs_abort_(void)
{
  logs_flush_sigsafe();
  qed_hs_raw_abort_();
}

#ifdef _WIN32
/** Take a filename and return a pointer to its final element.  This
 * function is called on __FILE__ to fix a MSVC nit where __FILE__
 * contains the full path to the file.  This is bad, because it
 * confuses users to find the home directory of the person who
 * compiled the binary in their warning messages.
 */
const char *
qed_hs_fix_source_file(const char *fname)
{
  const char *cp1, *cp2, *r;
  cp1 = strrchr(fname, '/');
  cp2 = strrchr(fname, '\\');
  if (cp1 && cp2) {
    r = (cp1<cp2)?(cp2+1):(cp1+1);
  } else if (cp1) {
    r = cp1+1;
  } else if (cp2) {
    r = cp2+1;
  } else {
    r = fname;
  }
  return r;
}
#endif /* defined(_WIN32) */
