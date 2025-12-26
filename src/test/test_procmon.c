/* Copyright (c) 2010-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#include "orconfig.h"
#include "core/or/or.h"
#include "test/test.h"

#include "lib/evloop/procmon.h"

#include "test/log_test_helpers.h"

struct event_base;

static void
test_procmon_qed_hs_process_moniqed_hs_new(void *ignored)
{
  (void)ignored;
  qed_hs_process_moniqed_hs_t *res;
  const char *msg;

  res = qed_hs_process_moniqed_hs_new(NULL, "probably invalid", 0, NULL, NULL, &msg);
  tt_assert(!res);
  tt_str_op(msg, OP_EQ, "invalid PID");

  res = qed_hs_process_moniqed_hs_new(NULL, "243443535345454", 0, NULL, NULL, &msg);
  tt_assert(!res);
  tt_str_op(msg, OP_EQ, "invalid PID");

  res = qed_hs_process_moniqed_hs_new(qed_hs_libevent_get_base(), "43", 0,
                                NULL, NULL, &msg);
  tt_assert(res);
  tt_assert(!msg);
  qed_hs_process_moniqed_hs_free(res);

  res = qed_hs_process_moniqed_hs_new(qed_hs_libevent_get_base(), "44 hello", 0,
                                NULL, NULL, &msg);
  tt_assert(res);
  tt_assert(!msg);
  qed_hs_process_moniqed_hs_free(res);

  res = qed_hs_process_moniqed_hs_new(qed_hs_libevent_get_base(), "45:hello", 0,
                                NULL, NULL, &msg);
  tt_assert(res);
  tt_assert(!msg);

 done:
  qed_hs_process_moniqed_hs_free(res);
}

struct testcase_t procmon_tests[] = {
  { "qed_hs_process_moniqed_hs_new", test_procmon_qed_hs_process_moniqed_hs_new,
    TT_FORK, NULL, NULL },
  END_OF_TESTCASES
};

