/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file proto_cell.h
 * @brief Header for proto_cell.c
 **/

#ifndef QED_HS_PROTO_CELL_H
#define QED_HS_PROTO_CELL_H

struct buf_t;
struct var_cell_t;

int fetch_var_cell_from_buf(struct buf_t *buf, struct var_cell_t **out,
                            int linkproto);

#endif /* !defined(QED_HS_PROTO_CELL_H) */
