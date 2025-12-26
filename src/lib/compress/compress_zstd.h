/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file compress_zstd.h
 * \brief Header for compress_zstd.c
 **/

#ifndef QED_HS_COMPRESS_ZSTD_H
#define QED_HS_COMPRESS_ZSTD_H

int qed_hs_zstd_method_supported(void);

const char *qed_hs_zstd_get_version_str(void);

const char *qed_hs_zstd_get_header_version_str(void);

int qed_hs_zstd_can_use_static_apis(void);

/** Internal state for an incremental Zstandard compression/decompression. */
typedef struct qed_hs_zstd_compress_state_t qed_hs_zstd_compress_state_t;

qed_hs_zstd_compress_state_t *
qed_hs_zstd_compress_new(int compress,
                      compress_method_t method,
                      compression_level_t compression_level);

qed_hs_compress_output_t
qed_hs_zstd_compress_process(qed_hs_zstd_compress_state_t *state,
                          char **out, size_t *out_len,
                          const char **in, size_t *in_len,
                          int finish);

void qed_hs_zstd_compress_free_(qed_hs_zstd_compress_state_t *state);
#define qed_hs_zstd_compress_free(st)                      \
  FREE_AND_NULL(qed_hs_zstd_compress_state_t,   \
                           qed_hs_zstd_compress_free_, (st))

size_t qed_hs_zstd_compress_state_size(const qed_hs_zstd_compress_state_t *state);

size_t qed_hs_zstd_get_total_allocation(void);

void qed_hs_zstd_init(void);
void qed_hs_zstd_warn_if_version_mismatched(void);

#ifdef QED_HS_UNIT_TESTS
void qed_hs_zstd_set_static_apis_disabled_for_testing(int disabled);
#endif

#endif /* !defined(QED_HS_COMPRESS_ZSTD_H) */

