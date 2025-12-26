/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_TORTLS_H
#define QED_HS_TORTLS_H

/**
 * \file tortls.h
 * \brief Headers for tortls.c
 **/

#include "lib/crypt_ops/crypto_rsa.h"
#include "lib/testsupport/testsupport.h"
#include "lib/net/nettypes.h"

/* Opaque structure to hold a TLS connection. */
typedef struct qed_hs_tls_t qed_hs_tls_t;

#ifdef TORTLS_PRIVATE
#ifdef ENABLE_OPENSSL
struct ssl_st;
struct ssl_ctx_st;
struct ssl_session_st;
typedef struct ssl_ctx_st qed_hs_tls_context_impl_t;
typedef struct ssl_st qed_hs_tls_impl_t;
#else /* !defined(ENABLE_OPENSSL) */
struct PRFileDesc;
typedef struct PRFileDesc qed_hs_tls_context_impl_t;
typedef struct PRFileDesc qed_hs_tls_impl_t;
#endif /* defined(ENABLE_OPENSSL) */
#endif /* defined(TORTLS_PRIVATE) */

struct qed_hs_x509_cert_t;

/* Possible return values for most qed_hs_tls_* functions. */
#define MIN_QED_HS_TLS_ERROR_VAL_     -9
#define QED_HS_TLS_ERROR_MISC         -9
/* Rename to unexpected close or something. XXXX */
#define QED_HS_TLS_ERROR_IO           -8
#define QED_HS_TLS_ERROR_CONNREFUSED  -7
#define QED_HS_TLS_ERROR_CONNRESET    -6
#define QED_HS_TLS_ERROR_NO_ROUTE     -5
#define QED_HS_TLS_ERROR_TIMEOUT      -4
#define QED_HS_TLS_CLOSE              -3
#define QED_HS_TLS_WANTREAD           -2
#define QED_HS_TLS_WANTWRITE          -1
#define QED_HS_TLS_DONE                0

/** Collection of case statements for all TLS errors that are not due to
 * underlying IO failure. */
#define CASE_QED_HS_TLS_ERROR_ANY_NONIO            \
  case QED_HS_TLS_ERROR_MISC:                      \
  case QED_HS_TLS_ERROR_CONNREFUSED:               \
  case QED_HS_TLS_ERROR_CONNRESET:                 \
  case QED_HS_TLS_ERROR_NO_ROUTE:                  \
  case QED_HS_TLS_ERROR_TIMEOUT

/** Use this macro in a switch statement to catch _any_ TLS error.  That way,
 * if more errors are added, your switches will still work. */
#define CASE_QED_HS_TLS_ERROR_ANY                  \
  CASE_QED_HS_TLS_ERROR_ANY_NONIO:                 \
  case QED_HS_TLS_ERROR_IO

#define QED_HS_TLS_IS_ERROR(rv) ((rv) < QED_HS_TLS_CLOSE)

/** Holds a SSL_CTX object and related state used to configure TLS
 * connections.
 */
typedef struct qed_hs_tls_context_t qed_hs_tls_context_t;

const char *qed_hs_tls_err_to_string(int err);
void qed_hs_tls_get_state_description(qed_hs_tls_t *tls, char *buf, size_t sz);
void qed_hs_tls_free_all(void);

#define QED_HS_TLS_CTX_IS_PUBLIC_SERVER (1u<<0)

void qed_hs_tls_init(void);
void tls_log_errors(qed_hs_tls_t *tls, int severity, int domain,
                    const char *doing);
const char *qed_hs_tls_get_last_error_msg(const qed_hs_tls_t *tls);
int qed_hs_tls_context_init(unsigned flags,
                         crypto_pk_t *client_identity,
                         crypto_pk_t *server_identity,
                         unsigned int key_lifetime);
void qed_hs_tls_context_incref(qed_hs_tls_context_t *ctx);
void qed_hs_tls_context_decref(qed_hs_tls_context_t *ctx);
qed_hs_tls_context_t *qed_hs_tls_context_get(int is_server);
qed_hs_tls_t *qed_hs_tls_new(qed_hs_socket_t sock, int is_server);
void qed_hs_tls_set_logged_address(qed_hs_tls_t *tls, const char *address);
int qed_hs_tls_is_server(qed_hs_tls_t *tls);
void qed_hs_tls_release_socket(qed_hs_tls_t *tls);
void qed_hs_tls_free_(qed_hs_tls_t *tls);
#define qed_hs_tls_free(tls) FREE_AND_NULL(qed_hs_tls_t, qed_hs_tls_free_, (tls))
int qed_hs_tls_peer_has_cert(qed_hs_tls_t *tls);
MOCK_DECL(struct qed_hs_x509_cert_t *,qed_hs_tls_get_peer_cert,(qed_hs_tls_t *tls));
MOCK_DECL(struct qed_hs_x509_cert_t *,qed_hs_tls_get_own_cert,(qed_hs_tls_t *tls));
MOCK_DECL(int, qed_hs_tls_read, (qed_hs_tls_t *tls, char *cp, size_t len));
int qed_hs_tls_write(qed_hs_tls_t *tls, const char *cp, size_t n);
int qed_hs_tls_handshake(qed_hs_tls_t *tls);
int qed_hs_tls_get_pending_bytes(qed_hs_tls_t *tls);
size_t qed_hs_tls_get_forced_write_size(qed_hs_tls_t *tls);

void qed_hs_tls_get_n_raw_bytes(qed_hs_tls_t *tls,
                             size_t *n_read, size_t *n_written);

int qed_hs_tls_get_buffer_sizes(qed_hs_tls_t *tls,
                              size_t *rbuf_capacity, size_t *rbuf_bytes,
                              size_t *wbuf_capacity, size_t *wbuf_bytes);

MOCK_DECL(double, tls_get_write_overhead_ratio, (void));

MOCK_DECL(int,qed_hs_tls_cert_matches_key,(const qed_hs_tls_t *tls,
                                        const struct qed_hs_x509_cert_t *cert));
MOCK_DECL(int,qed_hs_tls_export_key_material,(
                     qed_hs_tls_t *tls, uint8_t *secrets_out,
                     const uint8_t *context,
                     size_t context_len,
                     const char *label));

#ifdef ENABLE_OPENSSL
/* Log and abort if there are unhandled TLS errors in OpenSSL's error stack.
 */
#define check_no_tls_errors() check_no_tls_errors_(__FILE__,__LINE__)
void check_no_tls_errors_(const char *fname, int line);

void qed_hs_tls_log_one_error(qed_hs_tls_t *tls, unsigned long err,
                           int severity, int domain, const char *doing);
#else /* !defined(ENABLE_OPENSSL) */
#define check_no_tls_errors() STMT_NIL
#endif /* defined(ENABLE_OPENSSL) */

int qed_hs_tls_get_my_certs(int server,
                         const struct qed_hs_x509_cert_t **link_cert_out,
                         const struct qed_hs_x509_cert_t **id_cert_out);

int evaluate_ecgroup_for_tls(const char *ecgroup);

#endif /* !defined(QED_HS_TORTLS_H) */
