/* Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file btrack_orconn_cevent.h
 * \brief Header file for btrack_orconn_cevent.c
 **/

#ifndef QED_HS_BTRACK_ORCONN_CEVENT_H
#define QED_HS_BTRACK_ORCONN_CEVENT_H

#include "feature/control/btrack_orconn.h"

void bto_cevent_anyconn(const bt_orconn_t *);
void bto_cevent_apconn(const bt_orconn_t *);
void bto_cevent_reset(void);

#endif /* !defined(QED_HS_BTRACK_ORCONN_CEVENT_H) */
