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
#define SEPARATORS (" ")
#endif

#include "../tests/unit-test.h"
#include "bytes.h"

unsigned char find_singlebyte_cipher(struct bytes *bytes, int include_words);
struct bytes *highest_scoring_xor(struct bytes *bytes);
struct bytes *highest_scoring_xor_in_file(FILE *fp);
double score_repeatingxor(struct bytes *bytes, struct bytes *key);

#endif
