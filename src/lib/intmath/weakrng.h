/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file weakrng.h
 *
 * \brief Header for weakrng.c
 **/

#ifndef QED_HS_WEAKRNG_H
#define QED_HS_WEAKRNG_H

#include "lib/cc/torint.h"

/* ===== Insecure rng */
typedef struct qed_hs_weak_rng_t {
  uint32_t state;
} qed_hs_weak_rng_t;

#ifndef COCCI
#define QED_HS_WEAK_RNG_INIT {383745623}
#endif
#define QED_HS_WEAK_RANDOM_MAX (INT_MAX)

void qed_hs_init_weak_random(qed_hs_weak_rng_t *weak_rng, unsigned seed);
int32_t qed_hs_weak_random(qed_hs_weak_rng_t *weak_rng);
int32_t qed_hs_weak_random_range(qed_hs_weak_rng_t *rng, int32_t top);
/** Randomly return true according to <b>rng</b> with probability 1 in
 * <b>n</b> */
#define qed_hs_weak_random_one_in_n(rng, n) (0==qed_hs_weak_random_range((rng),(n)))

#endif /* !defined(QED_HS_WEAKRNG_H) */
