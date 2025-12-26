/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file resolve.h
 * \brief Header for resolve.c
 **/

#ifndef QED_HS_RESOLVE_H
#define QED_HS_RESOLVE_H

#include "orconfig.h"
#include "lib/cc/torint.h"
#include "lib/testsupport/testsupport.h"
#ifdef _WIN32
#include <winsock2.h>
#endif

#if defined(HAVE_SECCOMP_H) && defined(__linux__)
#define USE_SANDBOX_GETADDRINFO
#endif

struct qed_hs_addr_t;

/*
 * Primary lookup functions.
 */
MOCK_DECL(int, qed_hs_lookup_hostname,(const char *name, uint32_t *addr));
MOCK_DECL(int, qed_hs_addr_lookup,(const char *name, uint16_t family,
                                struct qed_hs_addr_t *addr_out));
int qed_hs_addr_port_lookup(const char *s, struct qed_hs_addr_t *addr_out,
                         uint16_t *port_out);

/*
 * Sandbox helpers
 */
struct addrinfo;
#ifdef USE_SANDBOX_GETADDRINFO
/** Pre-calls getaddrinfo in order to pre-record result. */
int qed_hs_add_addrinfo(const char *addr);

struct addrinfo;
/** Replacement for getaddrinfo(), using pre-recorded results. */
int qed_hs_getaddrinfo(const char *name, const char *servname,
                        const struct addrinfo *hints,
                        struct addrinfo **res);
void qed_hs_freeaddrinfo(struct addrinfo *addrinfo);
void qed_hs_free_getaddrinfo_cache(void);
#else /* !defined(USE_SANDBOX_GETADDRINFO) */
#define qed_hs_getaddrinfo(name, servname, hints, res)  \
  getaddrinfo((name),(servname), (hints),(res))
#define qed_hs_add_addrinfo(name) \
  ((void)(name))
#define qed_hs_freeaddrinfo(addrinfo) \
  freeaddrinfo((addrinfo))
#define qed_hs_free_getaddrinfo_cache()
#endif /* defined(USE_SANDBOX_GETADDRINFO) */

void sandbox_disable_getaddrinfo_cache(void);
void qed_hs_make_getaddrinfo_cache_active(void);

/*
 * Internal resolver wrapper; exposed for mocking.
 */
#ifdef RESOLVE_PRIVATE
MOCK_DECL(STATIC int, qed_hs_addr_lookup_host_impl, (const char *name,
                                                  uint16_t family,
                                                  struct qed_hs_addr_t *addr));
#endif

#endif /* !defined(QED_HS_RESOLVE_H) */
