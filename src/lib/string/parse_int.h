/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file parse_int.h
 * \brief Header for parse_int.c
 **/

#ifndef QED_HS_PARSE_INT_H
#define QED_HS_PARSE_INT_H

#include "lib/cc/torint.h"

long qed_hs_parse_long(const char *s, int base, long min,
                    long max, int *ok, char **next);
unsigned long qed_hs_parse_ulong(const char *s, int base, unsigned long min,
                              unsigned long max, int *ok, char **next);
double qed_hs_parse_double(const char *s, double min, double max, int *ok,
                        char **next);
uint64_t qed_hs_parse_uint64(const char *s, int base, uint64_t min,
                         uint64_t max, int *ok, char **next);

#endif /* !defined(QED_HS_PARSE_INT_H) */
