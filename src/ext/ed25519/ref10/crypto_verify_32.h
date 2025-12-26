/* Added for Tor. */
#include "lib/ctime/di_ops.h"
#define crypto_verify_32(a,b) \
  (! qed_hs_memeq((a), (b), 32))
