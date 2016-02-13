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
	int i, sum, j, k;
	struct bytes *block1;
	struct bytes *block2;
	i = sum = k = j = 0;

	if ((bytes->len / num_bytes) < 2)
		return bytes->len * num_bytes;

	block1 = bytes_create(num_bytes);
	block2 = bytes_create(num_bytes);

	for (k = 0, i = 0; i < num_bytes; i++, k++)
		block1->data[k] = bytes->data[i];
	for (k = 0, j = num_bytes; j < 2 * num_bytes; k++, j++)
		block2->data[k] = bytes->data[j];

	sum = edit_distance(block1, block2);

	bytes_put(block1);
	bytes_put(block2);

	return sum;
}

double edit_distance_avg(struct bytes *bytes, int num_bytes)
{
	int i, sum, num, j, start, end;
	struct bytes *block1;
	struct bytes *block2;
	i = sum = num = start = end = j = 0;

	if ((bytes->len / num_bytes) < 2)
		return (double)bytes->len * num_bytes;

	block1 = bytes_create(num_bytes);
	block2 = bytes_create(num_bytes);
	end = (bytes->len/num_bytes) * num_bytes;

	for (start = 0; start < (end - num_bytes);
			start += num_bytes * 2, num++) {
		int k = 0;
		for (k = 0, i = start; i < (start + num_bytes); i++, k++)
			block1->data[k] = bytes->data[i];
		for (k = 0, j = num_bytes + start; j < (start + 2  *num_bytes);
				k++, j++)
			block2->data[k] = bytes->data[j];

		sum += edit_distance(block1, block2);
	}

	bytes_put(block1);
	bytes_put(block2);

	return (double)sum / num;
}
