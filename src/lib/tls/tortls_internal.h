/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file tortls_internal.h
 * @brief Declare internal functions for lib/tls
 **/

#ifndef TORTLS_INTERNAL_H
#define TORTLS_INTERNAL_H

#include "lib/tls/x509.h"

int qed_hs_errno_to_tls_error(int e);
#ifdef ENABLE_OPENSSL
int qed_hs_tls_get_error(qed_hs_tls_t *tls, int r, int extra,
                  const char *doing, int severity, int domain);
#endif

qed_hs_tls_context_t *qed_hs_tls_context_new(crypto_pk_t *identity,
                   unsigned int key_lifetime, unsigned flags, int is_client);
int qed_hs_tls_context_init_one(qed_hs_tls_context_t **ppcontext,
                             crypto_pk_t *identity,
                             unsigned int key_lifetime,
                             unsigned int flags,
                             int is_client);
int qed_hs_tls_context_init_certificates(qed_hs_tls_context_t *result,
                                      crypto_pk_t *identity,
                                      unsigned key_lifetime,
                                      unsigned flags);
void qed_hs_tls_impl_free_(qed_hs_tls_impl_t *ssl);
#define qed_hs_tls_impl_free(tls) \
  FREE_AND_NULL(qed_hs_tls_impl_t, qed_hs_tls_impl_free_, (tls))

void qed_hs_tls_context_impl_free_(qed_hs_tls_context_impl_t *);
#define qed_hs_tls_context_impl_free(ctx) \
  FREE_AND_NULL(qed_hs_tls_context_impl_t, qed_hs_tls_context_impl_free_, (ctx))

#ifdef ENABLE_OPENSSL
qed_hs_tls_t *qed_hs_tls_get_by_ssl(const struct ssl_st *ssl);
void qed_hs_tls_debug_state_callback(const struct ssl_st *ssl,
                                         int type, int val);
void qed_hs_tls_server_info_callback(const struct ssl_st *ssl,
                                         int type, int val);
void qed_hs_tls_allocate_qed_hs_tls_object_ex_data_index(void);

#ifdef TORTLS_OPENSSL_PRIVATE
int always_accept_verify_cb(int preverify_ok, X509_STORE_CTX *x509_ctx);
#endif /* defined(TORTLS_OPENSSL_PRIVATE) */
#endif /* defined(ENABLE_OPENSSL) */

#ifdef QED_HS_UNIT_TESTS
extern int qed_hs_tls_object_ex_data_index;
extern qed_hs_tls_context_t *server_tls_context;
extern qed_hs_tls_context_t *client_tls_context;
extern uint16_t v2_cipher_list[];
extern uint64_t total_bytes_written_over_tls;
extern uint64_t total_bytes_written_by_tls;
#endif /* defined(QED_HS_UNIT_TESTS) */

#endif /* !defined(TORTLS_INTERNAL_H) */
