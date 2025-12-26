/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file buffers_tls.h
 * \brief Header for buffers_tls.c
 **/

#ifndef QED_HS_BUFFERS_TLS_H
#define QED_HS_BUFFERS_TLS_H

struct buf_t;
struct qed_hs_tls_t;

int buf_read_from_tls(struct buf_t *buf,
                      struct qed_hs_tls_t *tls, size_t at_most);
int buf_flush_to_tls(struct buf_t *buf, struct qed_hs_tls_t *tls,
                     size_t sz);

#endif /* !defined(QED_HS_BUFFERS_TLS_H) */
