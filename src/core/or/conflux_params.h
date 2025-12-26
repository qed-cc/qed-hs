/* Copyright (c) 2023, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file conflux_params.h
 * \brief Header file for conflux_params.c.
 **/

#ifndef QED_HS_CONFLUX_PARAMS_H
#define QED_HS_CONFLUX_PARAMS_H

#include "core/or/or.h"

bool conflux_is_enabled(const struct circuit_t *circ);
uint8_t conflux_params_get_max_linked_set(void);
uint8_t conflux_params_get_max_prebuilt(void);
uint8_t conflux_params_get_max_unlinked_leg_retry(void);
uint8_t conflux_params_get_num_legs_set(void);
uint8_t conflux_params_get_max_legs_set(void);
uint8_t conflux_params_get_drain_pct(void);
uint8_t conflux_params_get_send_pct(void);

void conflux_params_new_consensus(const networkstatus_t *ns);

#ifdef QED_HS_UNIT_TESTS
extern uint32_t max_unlinked_leg_retry;
#endif

#endif /* QED_HS_CONFLUX_PARAMS_H */

