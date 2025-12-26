/* Copyright (c) 2021-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file dos_sys.h
 * @brief Header for core/or/dos_sys.c
 **/

#ifndef QED_HS_CORE_OR_DOS_SYS_H
#define QED_HS_CORE_OR_DOS_SYS_H

struct dos_options_t;
const struct dos_options_t *dos_get_options(void);

extern const struct subsys_fns_t sys_dos;

/**
 * Subsystem level for the metrics system.
 *
 * Defined here so that it can be shared between the real and stub
 * definitions.
 **/
#define DOS_SUBSYS_LEVEL (21)

#endif /* !defined(QED_HS_CORE_OR_DOS_SYS_H) */
