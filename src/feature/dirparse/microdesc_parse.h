/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file microdesc_parse.h
 * \brief Header file for microdesc_parse.c.
 **/

#ifndef QED_HS_MICRODESC_PARSE_H
#define QED_HS_MICRODESC_PARSE_H

smartlist_t *microdescs_parse_from_string(const char *s, const char *eos,
                                          int allow_annotations,
                                          saved_location_t where,
                                          smartlist_t *invalid_digests_out);

#endif /* !defined(QED_HS_MICRODESC_PARSE_H) */
