#include "datamanip.h"

static int byte_dist(unsigned char b1, unsigned char b2)
{
	unsigned char b = 0;
	int sum = 0;

	for (b = b1 ^ b2; b > 0; b >>= 1)
		sum += b & 0x1;

	return sum;
}

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

struct bytes *xor_repeating_key(struct bytes *bytes, struct bytes *key)
{
	int i = 0;
	struct bytes *enciphered = bytes_create(bytes->len);

	for (i = 0; i < bytes->len; i++)
		enciphered->data[i] = xorbyte(bytes->data[i],
				key->data[i % key->len]);

	return enciphered;

}

int edit_distance(struct bytes *b1, struct bytes *b2)
{
	int i, sum;
	i = sum = 0;

	for (i = 0; i < b1->len; i++)
		sum += byte_dist(b1->data[i], b2->data[i]);

	return sum;
}

int edit_distance_count(struct bytes *bytes, int num_bytes)
{
	int i, sum, j;
	i = sum = j = 0;
	assert(j <= bytes->len);

	for (i = 0, j = num_bytes; i < num_bytes; i++, j++)
		sum += byte_dist(bytes->data[i], bytes->data[j]);

	return sum;

}
