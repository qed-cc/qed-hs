/* Copyright (c) 2018-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file log_sys.h
 * \brief Declare subsystem object for the logging module.
 **/

#ifndef QED_HS_TRACE_SYS_H
#define QED_HS_TRACE_SYS_H

extern const struct subsys_fns_t sys_tracing;

/**
 * Subsystem level for the tracing system.
 *
 * Defined here so that it can be shared between the real and stub
 * definitions.
 **/
#define TRACE_SUBSYS_LEVEL (-85)

#endif /* !defined(QED_HS_TRACE_SYS_H) */
