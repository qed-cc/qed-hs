/* Copyright (c) 2011-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file procmon.h
 * \brief Headers for procmon.c
 **/

#ifndef QED_HS_PROCMON_H
#define QED_HS_PROCMON_H

#include "lib/evloop/compat_libevent.h"

#include "lib/log/log.h"

typedef struct qed_hs_process_moniqed_hs_t qed_hs_process_moniqed_hs_t;

/* DOCDOC qed_hs_procmon_callback_t */
typedef void (*qed_hs_procmon_callback_t)(void *);

int qed_hs_validate_process_specifier(const char *process_spec,
                                   const char **msg);
qed_hs_process_moniqed_hs_t *qed_hs_process_moniqed_hs_new(struct event_base *base,
                                               const char *process_spec,
                                               log_domain_mask_t log_domain,
                                               qed_hs_procmon_callback_t cb,
                                               void *cb_arg,
                                               const char **msg);
void qed_hs_process_moniqed_hs_free_(qed_hs_process_moniqed_hs_t *procmon);
#define qed_hs_process_moniqed_hs_free(procmon) \
  FREE_AND_NULL(qed_hs_process_moniqed_hs_t, qed_hs_process_moniqed_hs_free_, (procmon))

#endif /* !defined(QED_HS_PROCMON_H) */

