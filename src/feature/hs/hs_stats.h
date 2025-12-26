/* Copyright (c) 2016-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file hs_stats.h
 * \brief Header file for hs_stats.c
 **/

#ifndef QED_HS_HS_STATS_H
#define QED_HS_HS_STATS_H

void hs_stats_note_introduce2_cell(void);
uint32_t hs_stats_get_n_introduce2_v3_cells(void);
void hs_stats_note_service_rendezvous_launch(void);
uint32_t hs_stats_get_n_rendezvous_launches(void);

#endif /* !defined(QED_HS_HS_STATS_H) */
