/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file malloc.h
 * \brief Headers for util_malloc.c
 **/

#ifndef QED_HS_UTIL_MALLOC_H
#define QED_HS_UTIL_MALLOC_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "lib/cc/compat_compiler.h"

/* Memory management */
void *qed_hs_malloc_(size_t size) ATTR_MALLOC;
void *qed_hs_malloc_zero_(size_t size) ATTR_MALLOC;
void *qed_hs_calloc_(size_t nmemb, size_t size) ATTR_MALLOC;
void *qed_hs_realloc_(void *ptr, size_t size);
void *qed_hs_reallocarray_(void *ptr, size_t size1, size_t size2);
char *qed_hs_strdup_(const char *s) ATTR_MALLOC;
char *qed_hs_strndup_(const char *s, size_t n)
  ATTR_MALLOC;
void *qed_hs_memdup_(const void *mem, size_t len)
  ATTR_MALLOC;
void *qed_hs_memdup_nulterm_(const void *mem, size_t len)
  ATTR_MALLOC;
void qed_hs_free_(void *mem);

/** Release memory allocated by qed_hs_malloc, qed_hs_realloc, qed_hs_strdup,
 * etc.  Unlike the free() function, the qed_hs_free() macro sets the
 * pointer value to NULL after freeing it.
 *
 * This is a macro.  If you need a function pointer to release memory from
 * qed_hs_malloc(), use qed_hs_free_().
 *
 * Note that this macro takes the address of the pointer it is going to
 * free and clear.  If that pointer is stored with a nonstandard
 * alignment (eg because of a "packed" pragma) it is not correct to use
 * qed_hs_free().
 */
#ifdef __GNUC__
#define qed_hs_free(p) STMT_BEGIN                                 \
    typeof(&(p)) qed_hs_free__tmpvar = &(p);                      \
    _Static_assert(!__builtin_types_compatible_p(typeof(*qed_hs_free__tmpvar), \
                                                 struct event *), \
                   "use qed_hs_event_free for struct event *");   \
    raw_free(*qed_hs_free__tmpvar);                               \
    *qed_hs_free__tmpvar=NULL;                                    \
  STMT_END
#else /* !defined(__GNUC__) */
#define qed_hs_free(p) STMT_BEGIN                                 \
  raw_free(p);                                                 \
  (p)=NULL;                                                    \
  STMT_END
#endif /* defined(__GNUC__) */

#define qed_hs_malloc(size)       qed_hs_malloc_(size)
#define qed_hs_malloc_zero(size)  qed_hs_malloc_zero_(size)
#define qed_hs_calloc(nmemb,size) qed_hs_calloc_(nmemb, size)
#define qed_hs_realloc(ptr, size) qed_hs_realloc_(ptr, size)
#define qed_hs_reallocarray(ptr, sz1, sz2) \
  qed_hs_reallocarray_((ptr), (sz1), (sz2))
#define qed_hs_strdup(s)          qed_hs_strdup_(s)
#define qed_hs_strndup(s, n)      qed_hs_strndup_(s, n)
#define qed_hs_memdup(s, n)       qed_hs_memdup_(s, n)
#define qed_hs_memdup_nulterm(s, n)       qed_hs_memdup_nulterm_(s, n)

/* Aliases for the underlying system malloc/realloc/free. Only use
 * them to indicate "I really want the underlying system function, I know
 * what I'm doing." */
#define raw_malloc  malloc
#define raw_realloc realloc
#define raw_free    free
#define raw_strdup  strdup

/* Helper macro: free a variable of type 'typename' using freefn, and
 * set the variable to NULL.
 */
#define FREE_AND_NULL(typename, freefn, var)                            \
  do {                                                                  \
    /* only evaluate (var) once. */                                     \
    typename **tmp__free__ptr ## freefn = &(var);                       \
    freefn(*tmp__free__ptr ## freefn);                                  \
    (*tmp__free__ptr ## freefn) = NULL;                                 \
  } while (0)

#ifdef UTIL_MALLOC_PRIVATE
STATIC int size_mul_check(const size_t x, const size_t y);
#endif

#endif /* !defined(QED_HS_UTIL_MALLOC_H) */
