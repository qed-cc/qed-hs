/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_X509_INTERNAL_H
#define QED_HS_X509_INTERNAL_H

/**
 * \file x509.h
 * \brief Internal headers for tortls.c
 **/

#include "lib/crypt_ops/crypto_rsa.h"
#include "lib/testsupport/testsupport.h"

/**
 * How skewed do we allow our clock to be with respect to certificates that
 * seem to be expired? (seconds)
 */
#define QED_HS_X509_PAST_SLOP (2*24*60*60)
/**
 * How skewed do we allow our clock to be with respect to certificates that
 * seem to come from the future? (seconds)
 */
#define  QED_HS_X509_FUTURE_SLOP (30*24*60*60)

MOCK_DECL(qed_hs_x509_cert_impl_t *, qed_hs_tls_create_certificate,
                                                   (crypto_pk_t *rsa,
                                                    crypto_pk_t *rsa_sign,
                                                    const char *cname,
                                                    const char *cname_sign,
                                                  unsigned int cert_lifetime));
MOCK_DECL(qed_hs_x509_cert_t *, qed_hs_x509_cert_new,
          (qed_hs_x509_cert_impl_t *x509_cert));

int qed_hs_x509_check_cert_lifetime_internal(int severity,
                                          const qed_hs_x509_cert_impl_t *cert,
                                          time_t now,
                                          int past_tolerance,
                                          int future_tolerance);

void qed_hs_x509_cert_impl_free_(qed_hs_x509_cert_impl_t *cert);
#define qed_hs_x509_cert_impl_free(cert) \
  FREE_AND_NULL(qed_hs_x509_cert_impl_t, qed_hs_x509_cert_impl_free_, (cert))
qed_hs_x509_cert_impl_t *qed_hs_x509_cert_impl_dup_(qed_hs_x509_cert_impl_t *cert);
#ifdef ENABLE_OPENSSL
int qed_hs_x509_cert_set_cached_der_encoding(qed_hs_x509_cert_t *cert);
#else
#define qed_hs_x509_cert_set_cached_der_encoding(cert) (0)
#endif

#endif /* !defined(QED_HS_X509_INTERNAL_H) */
