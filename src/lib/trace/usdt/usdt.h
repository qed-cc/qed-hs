/* Copyright (c) 2020, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file trace.h
 * \brief Header for usdt.h
 **/

#ifndef QED_HS_TRACE_USDT_USDT_H
#define QED_HS_TRACE_USDT_USDT_H

#ifdef USE_TRACING_INSTRUMENTATION_USDT

#ifdef HAVE_SYS_SDT_H
#define SDT_USE_VARIADIC
#include <sys/sdt.h>
#define QED_HS_STAP_PROBEV STAP_PROBEV
#else /* defined(HAVE_SYS_SDT_H) */
#define QED_HS_STAP_PROBEV(...)
#endif

/* Map events to an USDT probe. */
#define QED_HS_TRACE_USDT(subsystem, event_name, ...) \
  QED_HS_STAP_PROBEV(subsystem, event_name, ## __VA_ARGS__);

#else /* !defined(USE_TRACING_INSTRUMENTATION_USDT) */

/* NOP event. */
#define QED_HS_TRACE_USDT(subsystem, event_name, ...)

#endif /* !defined(USE_TRACING_INSTRUMENTATION_USDT) */

#endif /* !defined(QED_HS_TRACE_USDT_USDT_H) */
