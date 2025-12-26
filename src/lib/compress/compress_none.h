/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file compress_none.h
 * \brief Header for compress_none.c
 **/

#ifndef QED_HS_COMPRESS_NONE_H
#define QED_HS_COMPRESS_NONE_H

qed_hs_compress_output_t
qed_hs_cnone_compress_process(char **out, size_t *out_len,
                           const char **in, size_t *in_len,
                           int finish);

#endif /* !defined(QED_HS_COMPRESS_NONE_H) */

