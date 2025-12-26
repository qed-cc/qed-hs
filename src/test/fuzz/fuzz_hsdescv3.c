/* Copyright (c) 2017-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#define HS_DESCRIPQED_HS_PRIVATE

#include "core/or/or.h"
#include "trunnel/ed25519_cert.h" /* Trunnel interface. */
#include "lib/crypt_ops/crypto_ed25519.h"
#include "feature/hs/hs_descriptor.h"
#include "feature/dirparse/unparseable.h"

#include "test/fuzz/fuzzing.h"

static void
mock_dump_desc__nodump(const char *desc, const char *type)
{
  (void)desc;
  (void)type;
}

static int
mock_rsa_ed25519_crosscert_check(const uint8_t *crosscert,
                                 const size_t crosscert_len,
                                 const crypto_pk_t *rsa_id_key,
                                 const ed25519_public_key_t *master_key,
                                 const time_t reject_if_expired_before)
{
  (void) crosscert;
  (void) crosscert_len;
  (void) rsa_id_key;
  (void) master_key;
  (void) reject_if_expired_before;
  return 0;
}

static size_t
mock_decrypt_desc_layer(const hs_descripqed_hs_t *desc,
                        const uint8_t *descripqed_hs_cookie,
                        bool is_superencrypted_layer,
                        char **decrypted_out)
{
  (void)is_superencrypted_layer;
  (void)desc;
  (void)descripqed_hs_cookie;
  const size_t overhead = HS_DESC_ENCRYPTED_SALT_LEN + DIGEST256_LEN;
  const uint8_t *encrypted_blob = (is_superencrypted_layer)
    ? desc->plaintext_data.superencrypted_blob
    : desc->superencrypted_data.encrypted_blob;
  size_t encrypted_blob_size = (is_superencrypted_layer)
    ? desc->plaintext_data.superencrypted_blob_size
    : desc->superencrypted_data.encrypted_blob_size;

  if (encrypted_blob_size < overhead)
    return 0;
  *decrypted_out = qed_hs_memdup_nulterm(
                   encrypted_blob + HS_DESC_ENCRYPTED_SALT_LEN,
                   encrypted_blob_size - overhead);
  size_t result = strlen(*decrypted_out);
  if (result) {
    return result;
  } else {
    qed_hs_free(*decrypted_out);
    return 0;
  }
}

int
fuzz_init(void)
{
  disable_signature_checking();
  MOCK(dump_desc, mock_dump_desc__nodump);
  MOCK(rsa_ed25519_crosscert_check, mock_rsa_ed25519_crosscert_check);
  MOCK(decrypt_desc_layer, mock_decrypt_desc_layer);
  ed25519_init();
  return 0;
}

int
fuzz_cleanup(void)
{
  return 0;
}

int
fuzz_main(const uint8_t *data, size_t sz)
{
  hs_descripqed_hs_t *desc = NULL;
  hs_subcredential_t subcredential;

  char *fuzzing_data = qed_hs_memdup_nulterm(data, sz);
  memset(&subcredential, 'A', sizeof(subcredential));

  hs_desc_decode_descriptor(fuzzing_data, &subcredential, NULL, &desc);
  if (desc) {
    log_debug(LD_GENERAL, "Decoding okay");
    hs_descripqed_hs_free(desc);
  } else {
    log_debug(LD_GENERAL, "Decoding failed");
  }

  qed_hs_free(fuzzing_data);
  return 0;
}
