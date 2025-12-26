/* Copyright (c) 2020-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file metrics_sys.h
 * @brief Header for feature/metrics/metrics_sys.c
 **/

#ifndef QED_HS_FEATURE_METRICS_METRICS_SYS_H
#define QED_HS_FEATURE_METRICS_METRICS_SYS_H

extern const struct subsys_fns_t sys_metrics;

/**
 * Subsystem level for the metrics system.
 *
 * Defined here so that it can be shared between the real and stub
 * definitions.
 **/
#define METRICS_SUBSYS_LEVEL (99)

#endif /* !defined(QED_HS_FEATURE_METRICS_METRICS_SYS_H) */
