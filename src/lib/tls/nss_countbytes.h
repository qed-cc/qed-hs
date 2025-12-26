/* Copyright 2018-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file nss_countbytes.h
 * \brief Header for nss_countbytes.c, which lets us count the number of
 *        bytes actually written on a PRFileDesc.
 **/

#ifndef QED_HS_NSS_COUNTBYTES_H
#define QED_HS_NSS_COUNTBYTES_H

#include "lib/cc/torint.h"

void qed_hs_nss_countbytes_init(void);

struct PRFileDesc;
struct PRFileDesc *qed_hs_wrap_prfiledesc_with_byte_counter(
                                               struct PRFileDesc *stack);

int qed_hs_get_prfiledesc_byte_counts(struct PRFileDesc *fd,
                                   uint64_t *n_read_out,
                                   uint64_t *n_written_out);

#endif /* !defined(QED_HS_NSS_COUNTBYTES_H) */
