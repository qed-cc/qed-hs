/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_X509_H
#define QED_HS_X509_H

/**
 * \file x509.h
 * \brief Headers for tortls.c
 **/

#include "lib/crypt_ops/crypto_rsa.h"
#include "lib/testsupport/testsupport.h"

/* Opaque structure to hold an X509 certificate. */
typedef struct qed_hs_x509_cert_t qed_hs_x509_cert_t;

#ifdef ENABLE_NSS
typedef struct CERTCertificateStr qed_hs_x509_cert_impl_t;
#elif defined(ENABLE_OPENSSL)
typedef struct x509_st qed_hs_x509_cert_impl_t;
#endif

#ifdef QED_HS_X509_PRIVATE
/** Structure that we use for a single certificate. */
struct qed_hs_x509_cert_t {
  qed_hs_x509_cert_impl_t *cert;
#ifdef ENABLE_OPENSSL
  uint8_t *encoded;
  size_t encoded_len;
#endif
  unsigned pkey_digests_set : 1;
  common_digests_t cert_digests;
  common_digests_t pkey_digests;
};
#endif /* defined(QED_HS_X509_PRIVATE) */

void qed_hs_tls_pick_certificate_lifetime(time_t now,
                                       unsigned cert_lifetime,
                                       time_t *start_time_out,
                                       time_t *end_time_out);

#ifdef QED_HS_UNIT_TESTS
qed_hs_x509_cert_t *qed_hs_x509_cert_replace_expiration(
                                               const qed_hs_x509_cert_t *inp,
                                               time_t new_expiration_time,
                                               crypto_pk_t *signing_key);
#endif /* defined(QED_HS_UNIT_TESTS) */

qed_hs_x509_cert_t *qed_hs_x509_cert_dup(const qed_hs_x509_cert_t *cert);

void qed_hs_x509_cert_free_(qed_hs_x509_cert_t *cert);
#define qed_hs_x509_cert_free(c) \
  FREE_AND_NULL(qed_hs_x509_cert_t, qed_hs_x509_cert_free_, (c))
qed_hs_x509_cert_t *qed_hs_x509_cert_decode(const uint8_t *certificate,
                            size_t certificate_len);
void qed_hs_x509_cert_get_der(const qed_hs_x509_cert_t *cert,
                      const uint8_t **encoded_out, size_t *size_out);

const common_digests_t *qed_hs_x509_cert_get_id_digests(
                      const qed_hs_x509_cert_t *cert);
const common_digests_t *qed_hs_x509_cert_get_cert_digests(
                      const qed_hs_x509_cert_t *cert);

crypto_pk_t *qed_hs_tls_cert_get_key(qed_hs_x509_cert_t *cert);

int qed_hs_tls_cert_is_valid(int severity,
                          const qed_hs_x509_cert_t *cert,
                          const qed_hs_x509_cert_t *signing_cert,
                          time_t now,
                          int check_rsa_1024);

#endif /* !defined(QED_HS_X509_H) */
