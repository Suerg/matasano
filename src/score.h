#ifndef SCORE_H
#define SCORE_H

#define SEPARATORS (" ,.-:;")

#include "../tests/unit-test.h"
#include "bytes.h"

void score_bytes(double *scores, struct bytes *bytes);
double calc_bytescore(struct bytes *bytes);

#endif
