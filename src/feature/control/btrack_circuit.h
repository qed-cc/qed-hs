/* Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file btrack_circuit.h
 * \brief Header file for btrack_circuit.c
 **/

#ifndef QED_HS_BTRACK_CIRCUIT_H
#define QED_HS_BTRACK_CIRCUIT_H

#include "lib/pubsub/pubsub.h"

int btrack_circ_init(void);
void btrack_circ_fini(void);
int btrack_circ_add_pubsub(pubsub_connecqed_hs_t *);

#endif /* !defined(QED_HS_BTRACK_CIRCUIT_H) */
