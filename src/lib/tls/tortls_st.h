/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_TORTLS_ST_H
#define QED_HS_TORTLS_ST_H

/**
 * @file tortls_st.h
 * @brief Structure declarations for internal TLS types.
 *
 * These should generally be treated as opaque outside of the
 * lib/tls module.
 **/

#include "lib/net/socket.h"

#define QED_HS_TLS_MAGIC 0x71571571

typedef enum {
    QED_HS_TLS_ST_HANDSHAKE, QED_HS_TLS_ST_OPEN, QED_HS_TLS_ST_GOTCLOSE,
    QED_HS_TLS_ST_SENTCLOSE, QED_HS_TLS_ST_CLOSED, QED_HS_TLS_ST_RENEGOTIATE,
    QED_HS_TLS_ST_BUFFEREVENT
} qed_hs_tls_state_t;
#define qed_hs_tls_state_bitfield_t ENUM_BF(qed_hs_tls_state_t)

struct qed_hs_tls_context_t {
  int refcnt;
  qed_hs_tls_context_impl_t *ctx;
  struct qed_hs_x509_cert_t *my_link_cert;
  struct qed_hs_x509_cert_t *my_id_cert;
  struct qed_hs_x509_cert_t *my_auth_cert;
  crypto_pk_t *link_key;
  crypto_pk_t *auth_key;
};

/** Holds a SSL object and its associated data.  Members are only
 * accessed from within tortls.c.
 */
struct qed_hs_tls_t {
  uint32_t magic;
  qed_hs_tls_context_t *context; /** A link to the context object for this tls. */
  qed_hs_tls_impl_t *ssl; /**< An OpenSSL SSL object or NSS PRFileDesc. */
  qed_hs_socket_t socket; /**< The underlying file descriptor for this TLS
                        * connection. */
  char *address; /**< An address to log when describing this connection. */
  qed_hs_tls_state_bitfield_t state : 3; /**< The current SSL state,
                                       * depending on which operations
                                       * have completed successfully. */
  unsigned int isServer:1; /**< True iff this is a server-side connection */
#ifdef ENABLE_OPENSSL
  size_t wantwrite_n; /**< 0 normally, >0 if we returned wantwrite last
                       * time. */
  /** Last values retrieved from BIO_number_read()/write(); see
   * qed_hs_tls_get_n_raw_bytes() for usage.
   */
  unsigned long last_write_count;
  unsigned long last_read_count;
  /** Most recent error value from ERR_get_error(). */
  unsigned long last_error;
  /** If set, a callback to invoke whenever the client tries to renegotiate
   * the handshake. */
  void (*negotiated_callback)(qed_hs_tls_t *tls, void *arg);
  /** Argument to pass to negotiated_callback. */
  void *callback_arg;
#endif /* defined(ENABLE_OPENSSL) */
#ifdef ENABLE_NSS
  /** Last values retried from qed_hs_get_prfiledesc_byte_counts(). */
  uint64_t last_write_count;
  uint64_t last_read_count;
  long last_error;
#endif /* defined(ENABLE_NSS) */
};

#endif /* !defined(QED_HS_TORTLS_ST_H) */
