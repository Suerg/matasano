/*
 * Author: David Piper
 * cryptlib for matasano cryptopals.com challenges
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef S_SPLINT_S

#include <ctype.h>

#endif

#include "score.h"
#include "dataconvert.h"
#include "datamanip.h"
#include "bytes.h"
#include "cryptlib.h"

/*
 * Takes a c-string containing hex digits
 * produces a deciphered xor version.
 * Assumes it was enciphered with a single-byte xor cipher.
 */
void decipherxor(char *deciphered, char *hexstr)
{
	struct bytes *input = bytes_init_from_hexstr(hexstr);
	struct bytes *deciphered_bytes = highest_scoring_xor(input);

	bytestostr(deciphered, deciphered_bytes);

	bytes_put(deciphered_bytes);
	bytes_put(input);
}

void decipherxor_file(char *deciphered, char *fname)
{
	FILE *fp = NULL;
	struct bytes *deciphered_bytes = NULL;
	assert(fname != NULL);
	fp = fopen(fname, "r");
	assert(fp != NULL);

	deciphered_bytes = highest_scoring_xor_in_file(fp);
	bytestostr(deciphered, deciphered_bytes);

	fclose(fp);
	bytes_put(deciphered_bytes);
}

void encipher_repeating_key_xor(char *enciphered, char *str, char *key)
{
	struct bytes *bytes = bytes_init_from_str(str);
	struct bytes *key_bytes = bytes_init_from_str(key);
	struct bytes *enciphered_bytes = xor_repeating_key(bytes, key_bytes);

	bytestohexstr(enciphered, enciphered_bytes, 0);

	bytes_put(bytes);
	bytes_put(key_bytes);
	bytes_put(enciphered_bytes);
}

static int guess_keysize(struct bytes *enciphered)
{
	int min_dist = 100000;
	int i = 0;
	int keysize = 0;

	for (i = KEYSIZE; i > 0; i--) {
		int latest_dist = edit_distance_count(enciphered, i)/i;

		if (latest_dist < min_dist)
			min_dist = latest_dist;
	}

	return keysize;
}

static struct bytes **transpose_blocks(struct bytes **blocks, int keysize)
{
	int i, j;
	struct bytes **trans_blocks = malloc(keysize * sizeof(*trans_blocks));
	i = j = 0;

	for (i = 0; i < keysize; i++)
		trans_blocks[i] = bytes_create(keysize);

	for (i = 0; i < keysize; i++)
		for (j = 0; j < keysize; j++)
			trans_blocks[i]->data[j] = blocks[j]->data[i];

	return trans_blocks;
}

static struct bytes **make_keysize_blocks(struct bytes *enciphered,
					  int keysize)
{
	int i, j, k;
	struct bytes **blocks = malloc(keysize * sizeof(*blocks));
	i = j = k = 0;

	for (i = 0; i < keysize; i++)
		blocks[i] = bytes_create(keysize);

	for (i = 0; i < enciphered->len; i++)
		for (j = 0; j < keysize; j++)
			for (k = 0; k < keysize; k++)
				blocks[j]->data[k] =
					enciphered->data[i];
	return blocks;
}

static struct bytes *guess_key(struct bytes *enciphered, int keysize)
{
	int i;
	struct bytes *key = bytes_create(keysize);
	struct bytes **blocks = NULL;
	struct bytes **trans_blocks = NULL;
	blocks = make_keysize_blocks(enciphered, keysize);
	trans_blocks = transpose_blocks(blocks, keysize);

	for (i = 0; i < keysize; i++) {
		key->data[i] = find_singlebyte_cipher(trans_blocks[i]);
	}

	return key;
}

struct bytes *decipher_repeatingxor(struct bytes *enciphered)
{
	int keysize = 0;
	struct bytes *key = NULL;
	keysize = guess_keysize(enciphered);
	key = guess_key(enciphered, keysize);

	return xor_repeating_key(enciphered, key);
}

void decipher_repeatingxor_file(char *deciphered, char *fname)
{
}
