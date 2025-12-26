/* Copyright (c) 2003, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file x509_openssl.c
 * \brief Wrapper functions to present a consistent interface to
 * X.509 functions from OpenSSL.
 **/

#define QED_HS_X509_PRIVATE
#include "lib/tls/x509.h"
#include "lib/tls/x509_internal.h"
#include "lib/tls/tortls.h"
#include "lib/crypt_ops/crypto_rand.h"
#include "lib/crypt_ops/crypto_util.h"
#include "lib/crypt_ops/compat_openssl.h"

/* Some versions of OpenSSL declare SSL_get_selected_srtp_profile twice in
 * srtp.h. Suppress the GCC warning so we can build with -Wredundant-decl. */
DISABLE_GCC_WARNING("-Wredundant-decls")

#include <openssl/opensslv.h>

#ifdef OPENSSL_NO_EC
#error "We require OpenSSL with ECC support"
#endif

#include <openssl/err.h>
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

ENABLE_GCC_WARNING("-Wredundant-decls")

#include "lib/log/log.h"
#include "lib/log/util_bug.h"
#include "lib/ctime/di_ops.h"
#include "lib/encoding/time_fmt.h"

#include <stdlib.h>
#include <string.h>

#define X509_get_notBefore_const(cert) \
    X509_get0_notBefore(cert)
#define X509_get_notAfter_const(cert) \
    X509_get0_notAfter(cert)
#ifndef X509_get_notBefore
#define X509_get_notBefore(cert) \
    X509_getm_notBefore(cert)
#endif
#ifndef X509_get_notAfter
#define X509_get_notAfter(cert) \
    X509_getm_notAfter(cert)
#endif

/** Return a newly allocated X509 name with commonName <b>cname</b>. */
static X509_NAME *
qed_hs_x509_name_new(const char *cname)
{
  int nid;
  X509_NAME *name;
  /* LCOV_EXCL_BR_START : these branches will only fail on OOM errors */
  if (!(name = X509_NAME_new()))
    return NULL;
  if ((nid = OBJ_txt2nid("commonName")) == NID_undef) goto error;
  if (!(X509_NAME_add_entry_by_NID(name, nid, MBSTRING_ASC,
                                   (unsigned char*)cname, -1, -1, 0)))
    goto error;
  /* LCOV_EXCL_BR_STOP */
  return name;

  /* LCOV_EXCL_START : these lines will only execute on out of memory errors*/
 error:
  X509_NAME_free(name);
  return NULL;
  /* LCOV_EXCL_STOP */
}

/** Generate and sign an X509 certificate with the public key <b>rsa</b>,
 * signed by the private key <b>rsa_sign</b>.  The commonName of the
 * certificate will be <b>cname</b>; the commonName of the issuer will be
 * <b>cname_sign</b>. The cert will be valid for <b>cert_lifetime</b>
 * seconds, starting from some time in the past.
 *
 * Return a certificate on success, NULL on failure.
 */
MOCK_IMPL(X509 *,
qed_hs_tls_create_certificate,(crypto_pk_t *rsa,
                            crypto_pk_t *rsa_sign,
                            const char *cname,
                            const char *cname_sign,
                            unsigned int cert_lifetime))
{
  /* OpenSSL generates self-signed certificates with random 64-bit serial
   * numbers, so let's do that too. */
#define SERIAL_NUMBER_SIZE 8

  time_t start_time, end_time;
  BIGNUM *serial_number = NULL;
  unsigned char serial_tmp[SERIAL_NUMBER_SIZE];
  EVP_PKEY *sign_pkey = NULL, *pkey=NULL;
  X509 *x509 = NULL;
  X509_NAME *name = NULL, *name_issuer=NULL;

  qed_hs_tls_init();

  time_t now = time(NULL);

  qed_hs_tls_pick_certificate_lifetime(now, cert_lifetime,
                                    &start_time, &end_time);

  qed_hs_assert(rsa);
  qed_hs_assert(cname);
  qed_hs_assert(rsa_sign);
  qed_hs_assert(cname_sign);
  if (!(sign_pkey = crypto_pk_get_openssl_evp_pkey_(rsa_sign,1)))
    goto error;
  if (!(pkey = crypto_pk_get_openssl_evp_pkey_(rsa,0)))
    goto error;
  if (!(x509 = X509_new()))
    goto error;
  if (!(X509_set_version(x509, 2)))
    goto error;

  { /* our serial number is 8 random bytes. */
    crypto_rand((char *)serial_tmp, sizeof(serial_tmp));
    if (!(serial_number = BN_bin2bn(serial_tmp, sizeof(serial_tmp), NULL)))
      goto error;
    if (!(BN_to_ASN1_INTEGER(serial_number, X509_get_serialNumber(x509))))
      goto error;
  }

  if (!(name = qed_hs_x509_name_new(cname)))
    goto error;
  if (!(X509_set_subject_name(x509, name)))
    goto error;
  if (!(name_issuer = qed_hs_x509_name_new(cname_sign)))
    goto error;
  if (!(X509_set_issuer_name(x509, name_issuer)))
    goto error;

  if (!X509_time_adj(X509_get_notBefore(x509),0,&start_time))
    goto error;
  if (!X509_time_adj(X509_get_notAfter(x509),0,&end_time))
    goto error;
  if (!X509_set_pubkey(x509, pkey))
    goto error;

  if (!X509_sign(x509, sign_pkey, EVP_sha256()))
    goto error;

  goto done;
 error:
  if (x509) {
    X509_free(x509);
    x509 = NULL;
  }
 done:
  tls_log_errors(NULL, LOG_WARN, LD_NET, "generating certificate");
  if (sign_pkey)
    EVP_PKEY_free(sign_pkey);
  if (pkey)
    EVP_PKEY_free(pkey);
  if (serial_number)
    BN_clear_free(serial_number);
  if (name)
    X509_NAME_free(name);
  if (name_issuer)
    X509_NAME_free(name_issuer);
  return x509;

#undef SERIAL_NUMBER_SIZE
}

/** Set the 'encoded' and 'encoded_len' fields of "cert" from cert->cert. */
int
qed_hs_x509_cert_set_cached_der_encoding(qed_hs_x509_cert_t *cert)
{
  unsigned char *buf = NULL;
  int length = i2d_X509(cert->cert, &buf);

  if (length <= 0 || buf == NULL) {
    return -1;
  }
  cert->encoded_len = (size_t) length;
  cert->encoded = qed_hs_malloc(length);
  memcpy(cert->encoded, buf, length);
  OPENSSL_free(buf);
  return 0;
}

void
qed_hs_x509_cert_impl_free_(qed_hs_x509_cert_impl_t *cert)
{
  if (cert)
    X509_free(cert);
}

qed_hs_x509_cert_impl_t *
qed_hs_x509_cert_impl_dup_(qed_hs_x509_cert_impl_t *cert)
{
  if (cert)
    return X509_dup(cert);
  else
    return NULL;
}

/** Set *<b>encoded_out</b> and *<b>size_out</b> to <b>cert</b>'s encoded DER
 * representation and length, respectively. */
void
qed_hs_x509_cert_get_der(const qed_hs_x509_cert_t *cert,
                 const uint8_t **encoded_out, size_t *size_out)
{
  qed_hs_assert(cert);
  qed_hs_assert(encoded_out);
  qed_hs_assert(size_out);
  *encoded_out = cert->encoded;
  *size_out = cert->encoded_len;
}

/** Read a DER-encoded X509 cert, of length exactly <b>certificate_len</b>,
 * from a <b>certificate</b>.  Return a newly allocated qed_hs_x509_cert_t on
 * success and NULL on failure. */
qed_hs_x509_cert_t *
qed_hs_x509_cert_decode(const uint8_t *certificate, size_t certificate_len)
{
  X509 *x509;
  const unsigned char *cp = (const unsigned char *)certificate;
  qed_hs_x509_cert_t *newcert;
  qed_hs_assert(certificate);
  check_no_tls_errors();

  if (certificate_len > INT_MAX)
    goto err;

  x509 = d2i_X509(NULL, &cp, (int)certificate_len);

  if (!x509)
    goto err; /* Couldn't decode */
  if (cp - certificate != (int)certificate_len) {
    X509_free(x509);
    goto err; /* Didn't use all the bytes */
  }
  newcert = qed_hs_x509_cert_new(x509);
  if (!newcert) {
    goto err;
  }
  if (newcert->encoded_len != certificate_len ||
      fast_memneq(newcert->encoded, certificate, certificate_len)) {
    /* Cert wasn't in DER */
    qed_hs_x509_cert_free(newcert);
    goto err;
  }
  return newcert;
 err:
  tls_log_errors(NULL, LOG_INFO, LD_CRYPTO, "decoding a certificate");
  return NULL;
}

/**
 * Return a newly allocated copy of the public key that a certificate
 * certifies. Watch out! This returns NULL if the cert's key is not RSA.
 */
crypto_pk_t *
qed_hs_tls_cert_get_key(qed_hs_x509_cert_t *cert)
{
  crypto_pk_t *result = NULL;
  EVP_PKEY *pkey = X509_get_pubkey(cert->cert);
  RSA *rsa;
  if (!pkey)
    return NULL;
  rsa = EVP_PKEY_get1_RSA(pkey);
  if (!rsa) {
    EVP_PKEY_free(pkey);
    return NULL;
  }
  result = crypto_new_pk_from_openssl_rsa_(rsa);
  EVP_PKEY_free(pkey);
  return result;
}

/** Check whether <b>cert</b> is well-formed, currently live, and correctly
 * signed by the public key in <b>signing_cert</b>.  If <b>check_rsa_1024</b>,
 * make sure that it has an RSA key with 1024 bits; otherwise, just check that
 * the key is long enough. Return 1 if the cert is good, and 0 if it's bad or
 * we couldn't check it. */
int
qed_hs_tls_cert_is_valid(int severity,
                      const qed_hs_x509_cert_t *cert,
                      const qed_hs_x509_cert_t *signing_cert,
                      time_t now,
                      int check_rsa_1024)
{
  check_no_tls_errors();
  EVP_PKEY *cert_key;
  int r, key_ok = 0;

  if (!signing_cert || !cert)
    goto bad;

  EVP_PKEY *signing_key = X509_get_pubkey(signing_cert->cert);
  if (!signing_key)
    goto bad;
  r = X509_verify(cert->cert, signing_key);
  EVP_PKEY_free(signing_key);
  if (r <= 0)
    goto bad;

  /* okay, the signature checked out right.  Now let's check the check the
   * lifetime. */
  if (qed_hs_x509_check_cert_lifetime_internal(severity, cert->cert, now,
                                            QED_HS_X509_PAST_SLOP,
                                            QED_HS_X509_FUTURE_SLOP) < 0)
    goto bad;

  cert_key = X509_get_pubkey(cert->cert);
  if (check_rsa_1024 && cert_key) {
    RSA *rsa = EVP_PKEY_get1_RSA(cert_key);
    if (rsa && RSA_bits(rsa) == 1024) {
      key_ok = 1;
    } else {
      log_fn(severity, LD_CRYPTO, "Invalid certificate: Key is not RSA1024.");
    }

    if (rsa)
      RSA_free(rsa);
  } else if (cert_key) {
    int min_bits = 1024;
#ifdef EVP_PKEY_EC
    if (EVP_PKEY_base_id(cert_key) == EVP_PKEY_EC)
      min_bits = 128;
#endif
    if (EVP_PKEY_bits(cert_key) >= min_bits)
      key_ok = 1;
  }
  EVP_PKEY_free(cert_key);
  if (!key_ok)
    goto bad;

  /* XXXX compare DNs or anything? */

  return 1;
 bad:
  tls_log_errors(NULL, LOG_INFO, LD_CRYPTO, "checking a certificate");
  return 0;
}

/** Warn that a certificate lifetime extends through a certain range. */
static void
log_cert_lifetime(int severity, const X509 *cert, const char *problem,
                  time_t now)
{
  BIO *bio = NULL;
  BUF_MEM *buf;
  char *s1=NULL, *s2=NULL;
  char mytime[33];
  struct tm tm;
  size_t n;

  if (problem)
    qed_hs_log(severity, LD_GENERAL,
        "Certificate %s. Either their clock is set wrong, or your clock "
        "is wrong.",
           problem);

  if (!(bio = BIO_new(BIO_s_mem()))) {
    log_warn(LD_GENERAL, "Couldn't allocate BIO!"); goto end;
  }
  if (!(ASN1_TIME_print(bio, X509_get_notBefore_const(cert)))) {
    tls_log_errors(NULL, LOG_WARN, LD_NET, "printing certificate lifetime");
    goto end;
  }
  BIO_get_mem_ptr(bio, &buf);
  s1 = qed_hs_strndup(buf->data, buf->length);

  (void)BIO_reset(bio);
  if (!(ASN1_TIME_print(bio, X509_get_notAfter_const(cert)))) {
    tls_log_errors(NULL, LOG_WARN, LD_NET, "printing certificate lifetime");
    goto end;
  }
  BIO_get_mem_ptr(bio, &buf);
  s2 = qed_hs_strndup(buf->data, buf->length);

  n = strftime(mytime, 32, "%b %d %H:%M:%S %Y UTC", qed_hs_gmtime_r(&now, &tm));
  if (n > 0) {
    qed_hs_log(severity, LD_GENERAL,
        "(certificate lifetime runs from %s through %s. Your time is %s.)",
        s1,s2,mytime);
  } else {
    qed_hs_log(severity, LD_GENERAL,
        "(certificate lifetime runs from %s through %s. "
        "Couldn't get your time.)",
        s1, s2);
  }

 end:
  /* Not expected to get invoked */
  tls_log_errors(NULL, LOG_WARN, LD_NET, "getting certificate lifetime");
  if (bio)
    BIO_free(bio);
  qed_hs_free(s1);
  qed_hs_free(s2);
}

/** Helper: check whether <b>cert</b> is expired give or take
 * <b>past_tolerance</b> seconds, or not-yet-valid give or take
 * <b>future_tolerance</b> seconds.  (Relative to the current time
 * <b>now</b>.)  If it is live, return 0.  If it is not live, log a message
 * and return -1. */
int
qed_hs_x509_check_cert_lifetime_internal(int severity, const X509 *cert,
                                      time_t now,
                                      int past_tolerance, int future_tolerance)
{
  time_t t;

  t = now + future_tolerance;
  if (X509_cmp_time(X509_get_notBefore_const(cert), &t) > 0) {
    log_cert_lifetime(severity, cert, "not yet valid", now);
    return -1;
  }
  t = now - past_tolerance;
  if (X509_cmp_time(X509_get_notAfter_const(cert), &t) < 0) {
    log_cert_lifetime(severity, cert, "already expired", now);
    return -1;
  }

  return 0;
}

#ifdef QED_HS_UNIT_TESTS
/* Testing only: return a new x509 cert with the same contents as <b>inp</b>,
   but with the expiration time <b>new_expiration_time</b>, signed with
   <b>signing_key</b>. */
STATIC qed_hs_x509_cert_t *
qed_hs_x509_cert_replace_expiration(const qed_hs_x509_cert_t *inp,
                                 time_t new_expiration_time,
                                 crypto_pk_t *signing_key)
{
  X509 *newc = X509_dup(inp->cert);
  X509_time_adj(X509_get_notAfter(newc), 0, &new_expiration_time);
  EVP_PKEY *pk = crypto_pk_get_openssl_evp_pkey_(signing_key, 1);
  qed_hs_assert(X509_sign(newc, pk, EVP_sha256()));
  EVP_PKEY_free(pk);
  return qed_hs_x509_cert_new(newc);
}
#endif /* defined(QED_HS_UNIT_TESTS) */
