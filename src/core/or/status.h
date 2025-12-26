/* Copyright (c) 2010-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file status.h
 * @brief Header for status.c
 **/

#ifndef QED_HS_STATUS_H
#define QED_HS_STATUS_H

#include "lib/testsupport/testsupport.h"

void note_connection(bool inbound, const connection_t *conn);
void note_circ_closed_for_unrecognized_cells(time_t n_seconds,
                                             uint32_t n_cells);

int log_heartbeat(time_t now);

#ifdef STATUS_PRIVATE
STATIC int count_circuits(void);
STATIC char *secs_to_uptime(long secs);
STATIC char *bytes_to_usage(uint64_t bytes);
#endif

#endif /* !defined(QED_HS_STATUS_H) */
