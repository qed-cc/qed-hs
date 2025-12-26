
#ifndef TRUNNEL_LOCAL_H_INCLUDED
#define TRUNNEL_LOCAL_H_INCLUDED

#include "lib/crypt_ops/crypto_util.h"
#include "lib/malloc/malloc.h"
#include "lib/log/util_bug.h"

#define trunnel_malloc qed_hs_malloc
#define trunnel_calloc qed_hs_calloc
#define trunnel_strdup qed_hs_strdup
#define trunnel_free_ qed_hs_free_
#define trunnel_realloc qed_hs_realloc
#define trunnel_reallocarray qed_hs_reallocarray
#define trunnel_assert qed_hs_assert
#define trunnel_memwipe(mem, len) memwipe((mem), 0, (len))
#define trunnel_abort qed_hs_abort_

#endif
