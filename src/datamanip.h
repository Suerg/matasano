#ifndef DATAMANIP_H
#define DATAMANIP_H

#include "../tests/unit-test.h"
#include "bytes.h"

void xortwo(struct bytes *xored, struct bytes *b1,
		struct bytes *b2);
unsigned char xorbyte(unsigned char byte, unsigned char xorwith);
void xorbytes(struct bytes *xored, struct bytes *bytes, unsigned char xorwith);
struct bytes *xor_repeating_key(struct bytes *bytes, struct bytes *key);
int edit_distance(struct bytes *b1, struct bytes *b2);
int edit_distance_count(struct bytes *bytes, int num_bytes);

#endif
