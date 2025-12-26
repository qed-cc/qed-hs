/* Copyright (c) 2017-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_RNG_TEST_HELPERS_H
#define QED_HS_RNG_TEST_HELPERS_H

#include "core/or/or.h"

void testing_enable_deterministic_rng(void);
void testing_enable_reproducible_rng(void);
void testing_enable_prefilled_rng(const void *buffer, size_t buflen);

void testing_prefilled_rng_reset(void);

void testing_disable_rng_override(void);

void testing_disable_reproducible_rng(void);
#define testing_disable_deterministic_rng() \
  testing_disable_rng_override()
#define testing_disable_prefilled_rng() \
  testing_disable_rng_override()

void testing_dump_reproducible_rng_seed(void);

#endif /* !defined(QED_HS_RNG_TEST_HELPERS_H) */
