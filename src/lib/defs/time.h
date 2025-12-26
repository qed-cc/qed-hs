/* Copyright (c) 2001, Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_TIME_DEFS_H
#define QED_HS_TIME_DEFS_H

/**
 * \file time.h
 *
 * \brief Definitions for timing-related constants.
 **/

/** How many microseconds per second */
#define QED_HS_USEC_PER_SEC (1000000)
/** How many nanoseconds per microsecond */
#define QED_HS_NSEC_PER_USEC (1000)
/** How many nanoseconds per millisecond */
#define QED_HS_NSEC_PER_MSEC (1000*1000)

#endif /* !defined(QED_HS_TIME_DEFS_H) */
