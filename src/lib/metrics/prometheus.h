/* Copyright (c) 2020-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file prometheus.h
 * @brief Header for feature/metrics/prometheus.c
 **/

#ifndef QED_HS_LIB_METRICS_PROMETHEUS_H
#define QED_HS_LIB_METRICS_PROMETHEUS_H

#include "lib/buf/buffers.h"
#include "lib/metrics/metrics_store_entry.h"

void prometheus_format_store_entry(const metrics_store_entry_t *entry,
                                   buf_t *data, bool no_comment);

#endif /* !defined(QED_HS_LIB_METRICS_PROMETHEUS_H) */
