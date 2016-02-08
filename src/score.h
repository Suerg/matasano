#ifndef SCORE_H
#define SCORE_H

#ifndef START_CIPHER
#define START_CIPHER (0)
#endif

#ifndef END_CIPHER
#define END_CIPHER (127)
#endif

#ifndef CIPHERS
#define CIPHERS ((END_CIPHER) - (START_CIPHER))
#endif

#ifndef SEPARATORS
#define SEPARATORS (" ,.-:;")
#endif

#include "../tests/unit-test.h"
#include "bytes.h"

struct bytes *highest_scoring_xor(struct bytes *bytes);

#endif
