/* Copyright (c) 2019-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file congestion_control_vegas.h
 * \brief Private-ish APIs for the QED_HS_VEGAS congestion control algorithm
 **/

#ifndef QED_HS_CONGESTION_CONTROL_VEGAS_H
#define QED_HS_CONGESTION_CONTROL_VEGAS_H

#include "core/or/crypt_path_st.h"
#include "core/or/circuit_st.h"

extern double cc_stats_vegas_exit_ss_cwnd_ma;
extern double cc_stats_vegas_exit_ss_bdp_ma;
extern double cc_stats_vegas_exit_ss_inc_ma;
extern double cc_stats_vegas_gamma_drop_ma;
extern double cc_stats_vegas_delta_drop_ma;
extern double cc_stats_vegas_ss_csig_blocked_ma;
extern double cc_stats_vegas_csig_blocked_ma;
extern uint64_t cc_stats_vegas_above_delta;
extern uint64_t cc_stats_vegas_above_ss_cwnd_max;

extern double cc_stats_vegas_csig_alpha_ma;
extern double cc_stats_vegas_csig_beta_ma;
extern double cc_stats_vegas_csig_delta_ma;

extern double cc_stats_vegas_ss_queue_ma;
extern double cc_stats_vegas_queue_ma;
extern double cc_stats_vegas_bdp_ma;

extern uint64_t cc_stats_vegas_below_ss_inc_floor;
extern uint64_t cc_stats_vegas_circ_exited_ss;

/* Processing SENDME cell. */
int congestion_control_vegas_process_sendme(struct congestion_control_t *cc,
                                            const circuit_t *circ);
void congestion_control_vegas_set_params(struct congestion_control_t *cc,
                                         cc_path_t path);

/* Private section starts. */
#ifdef QED_HS_CONGESTION_CONTROL_VEGAS_PRIVATE

/*
 * Unit tests declaractions.
 */
#ifdef QED_HS_UNIT_TESTS

#endif /* defined(QED_HS_UNIT_TESTS) */

#endif /* defined(QED_HS_CONGESTION_CONTROL_VEGAS_PRIVATE) */

#endif /* !defined(QED_HS_CONGESTION_CONTROL_VEGAS_H) */
