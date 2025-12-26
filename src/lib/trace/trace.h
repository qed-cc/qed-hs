/* Copyright (c) 2020-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file trace.h
 * \brief Header for trace.c
 **/

#ifndef QED_HS_LIB_TRACE_TRACE_H
#define QED_HS_LIB_TRACE_TRACE_H

#include "orconfig.h"

void qed_hs_trace_init(void);
void qed_hs_trace_free_all(void);

#ifdef HAVE_TRACING

#include "lib/log/log.h"

static inline void
tracing_log_warning(void)
{
  log_warn(LD_GENERAL,
           "Tracing capabilities have been built in. If this is NOT on "
           "purpose, your tor is NOT safe to run.");
}

#else /* !defined(HAVE_TRACING) */

/* NOP it. */
#define tracing_log_warning()

#endif /* defined(HAVE_TRACING) */

#endif /* !defined(QED_HS_LIB_TRACE_TRACE_H) */
