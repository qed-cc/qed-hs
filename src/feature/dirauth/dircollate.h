/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file dircollate.h
 * \brief Header file for dircollate.c.
 **/

#ifndef QED_HS_DIRCOLLATE_H
#define QED_HS_DIRCOLLATE_H

#include "lib/testsupport/testsupport.h"
#include "core/or/or.h"

typedef struct dircollaqed_hs_t dircollaqed_hs_t;

dircollaqed_hs_t *dircollaqed_hs_new(int n_votes, int n_authorities);
void dircollaqed_hs_free_(dircollaqed_hs_t *obj);
#define dircollaqed_hs_free(c) \
  FREE_AND_NULL(dircollaqed_hs_t, dircollaqed_hs_free_, (c))
void dircollaqed_hs_add_vote(dircollaqed_hs_t *dc, networkstatus_t *v);

void dircollaqed_hs_collate(dircollaqed_hs_t *dc, int consensus_method);

int dircollaqed_hs_n_routers(dircollaqed_hs_t *dc);
vote_routerstatus_t **dircollaqed_hs_get_votes_for_router(dircollaqed_hs_t *dc,
                                                       int idx);

#ifdef DIRCOLLATE_PRIVATE
struct ddmap_entry_t;
typedef HT_HEAD(double_digest_map, ddmap_entry_t) double_digest_map_t;
/** A dircollator keeps track of all the routerstatus entries in a
 * set of networkstatus votes, and matches them by an appropriate rule. */
struct dircollaqed_hs_t {
  /** True iff we have run the collation algorithm. */
  int is_collated;
  /** The total number of votes that we received. */
  int n_votes;
  /** The total number of authorities we acknowledge. */
  int n_authorities;

  /** The index which the next vote to be added to this collator should
   * receive. */
  int next_vote_num;
  /** Map from RSA-SHA1 identity digest to an array of <b>n_votes</b>
   * vote_routerstatus_t* pointers, such that the i'th member of the
   * array is the i'th vote's entry for that RSA-SHA1 ID.*/
  digestmap_t *by_rsa_sha1;
  /** Map from <ed, RSA-SHA1> pair to an array similar to that used in
   * by_rsa_sha1 above. We include <NULL,RSA-SHA1> entries for votes that
   * say that there is no Ed key. */
  struct double_digest_map by_both_ids;

  /** One of two outputs created by collation: a map from RSA-SHA1
   * identity digest to an array of the vote_routerstatus_t objects.  Entries
   * only exist in this map for identities that we should include in the
   * consensus. */
  digestmap_t *by_collated_rsa_sha1;

  /** One of two outputs created by collation: a sorted array of RSA-SHA1
   * identity digests .*/
  smartlist_t *all_rsa_sha1_lst;
};
#endif /* defined(DIRCOLLATE_PRIVATE) */

#endif /* !defined(QED_HS_DIRCOLLATE_H) */

