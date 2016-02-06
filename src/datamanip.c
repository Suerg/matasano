#include "datamanip.h"

void xortwo(struct bytes *xored, struct bytes *b1, struct bytes *b2)
{
	int i = 0;

	for (i = 0; i < b1->len; i++)
		xored->data[i] = b1->data[i] ^ b2->data[i];
}

unsigned char xorbyte(unsigned char byte, unsigned char xorwith)
{
	return byte ^ xorwith;
}

void xorbytes(struct bytes *xored, struct bytes *bytes, unsigned char xorwith)
{
	int i = 0;

	for (i = 0; i < bytes->len; i++)
		xored->data[i] = xorbyte(bytes->data[i], xorwith);
}
