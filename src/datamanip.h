#ifndef DATAMANIP_H
#define DATAMANIP_H

#include "../tests/unit-test.h"
#include "bytes.h"

void xortwo(struct bytes *xored, struct bytes *b1,
		struct bytes *b2);
unsigned char xorbyte(unsigned char byte, unsigned char xorwith);
void xorbytes(struct bytes *xored, struct bytes *bytes, unsigned char xorwith);

#endif
