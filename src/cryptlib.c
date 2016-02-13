/*
 * Author: David Piper
 * cryptlib for matasano cryptopals.com challenges
 */

#include <math.h>
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

static int is_keysize_local_min(struct bytes *enciphered, int keysize);
static double avg_dist_among_minima(struct bytes *enciphered);
static double std_dev_dist_among_minima(struct bytes *enciphered);
static void find_pot_keysizes(int **pot_keysizes, int *len,
				struct bytes *enciphered);

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

static int is_keysize_local_min(struct bytes *enciphered, int keysize)
{
	double itself_dist = edit_distance_avg(enciphered, keysize) / keysize;
	if (keysize == 1 && KEYSIZE > 1) {
		double next_dist = edit_distance_avg(enciphered, keysize + 1)
			/ (keysize + 1);

		return itself_dist < next_dist;
	} else if (keysize == KEYSIZE) {
		double prev_dist = edit_distance_avg(enciphered, keysize - 1)
			/ (keysize - 1);

		return itself_dist < prev_dist;
	} else {
		double prev_dist = edit_distance_avg(enciphered, keysize - 1)
			/ (keysize - 1);
		double next_dist = edit_distance_avg(enciphered, keysize + 1)
			/ (keysize + 1);

		return (itself_dist < prev_dist) && (itself_dist < next_dist);
	}
}

static double avg_dist_among_minima(struct bytes *enciphered)
{
	double sum, avg, sq_dist;
	int i, j, keysize, count;
	i = j = keysize = count = 0;
	sum = avg = sq_dist = 0.0;

	for (i = 1, j = 0; i <= KEYSIZE; i++) {
		double latest_dist_avg = (double)
			edit_distance_avg(enciphered, i) / i;
		double latest_dist = latest_dist_avg;
		int is_local_minima = is_keysize_local_min(enciphered, i);

		if (is_local_minima) {
			sum += latest_dist;
			count++;
		}

	}

	if (count > 0)
		return (double)sum / count;
	else
		return -1;
}

static double std_dev_dist_among_minima(struct bytes *enciphered)
{

	double avg, sq_dist;
	int i, j, keysize, count;
	i = j = keysize = count = 0;
	avg = sq_dist = 0.0;

	avg = avg_dist_among_minima(enciphered);

	for (i = 1, j = 0; i <= KEYSIZE; i++) {
		double latest_dist_avg = (double)
			edit_distance_avg(enciphered, i) / i;
		double latest_dist = latest_dist_avg;
		int is_local_minima = is_keysize_local_min(enciphered, i);

		if (is_local_minima) {
			sq_dist += pow(latest_dist - avg, 2);
			count++;
		}

	}

	if (count > 0)
		return sqrt(sq_dist / count);
	else
		return -1;
}

static void find_pot_keysizes(int **pot_keysizes, int *len,
		struct bytes *enciphered)
{
	int i, j, keysize;
	*pot_keysizes = malloc(NUM_KEYSIZES * sizeof(**pot_keysizes));
	i = j = keysize = 0;

	for (i = 1, j = 0; i <= KEYSIZE; i++) {

		double latest_dist_avg = (double)
			edit_distance_avg(enciphered, i)/i;
		double latest_dist = latest_dist_avg;
		double avg = avg_dist_among_minima(enciphered);
		double stddev = std_dev_dist_among_minima(enciphered);
		int is_less_stddev = latest_dist <= (avg - stddev);

		if (is_less_stddev) {

			if (j < NUM_KEYSIZES)
				(*pot_keysizes)[j++] = i;
		}
	}

	*len = j;
}

/*
 *int find_keysize(struct bytes *enciphered)
 *{
 *        int *pot_keysizes = NULL;
 *        int len, keysize;
 *        len = keysize = 0;
 *        find_pot_keysizes(&pot_keysizes, &len, enciphered);
 *
 *        free(pot_keysizes);
 *
 *        return keysize;
 *}
 */

static struct bytes **transpose_blocks(struct bytes *enciphered, struct bytes **blocks, int keysize)
{
	int i, j;
	int num_blocks = enciphered->len / keysize;
	int rem = enciphered->len % keysize;
	struct bytes **trans_blocks = NULL;
	if (rem != 0)
		num_blocks++;
	i = j = 0;
	trans_blocks = malloc(keysize * sizeof(*trans_blocks));


	if (rem) {
		for (i = 0; i < rem - 1; i++)
			trans_blocks[i] = bytes_create(num_blocks);
		for (i = rem - 1; i < keysize; i++)
			trans_blocks[i] = bytes_create(num_blocks - 1);
	} else {
		for (i = 0; i < keysize; i++)
			trans_blocks[i] = bytes_create(num_blocks);
	}

	for (i = 0; i < keysize; i++)
		for (j = 0; j < trans_blocks[i]->len; j++)
			trans_blocks[i]->data[j] = blocks[j]->data[i];

	return trans_blocks;
}

static struct bytes **make_keysize_blocks(struct bytes *enciphered,
					  int keysize)
{
	int i, j, k;
	int num_blocks = enciphered->len / keysize;
	struct bytes **blocks = NULL;
	int rem = enciphered->len % keysize;
	if (rem != 0)
		num_blocks++;
	blocks = malloc(num_blocks * sizeof(*blocks));
	i = j = k = 0;

	for (i = 0; i < num_blocks; i++)
		blocks[i] = bytes_create(keysize);

	for (i = 0, k = 0; k < num_blocks - 1; i++) {
		blocks[k]->data[i % keysize] = enciphered->data[i];
		if ((i + 1) % keysize == 0)
			k++;
	}

	for(; i % keysize < rem; i++)
		blocks[k]->data[i % keysize] = enciphered->data[i];

	return blocks;
}

static struct bytes *guess_key(struct bytes *enciphered, int keysize)
{
	int i;
	struct bytes *key = bytes_create(keysize);
	struct bytes **blocks = NULL;
	struct bytes **trans_blocks = NULL;
	int num_blocks = enciphered->len / keysize;
	int rem = enciphered->len % keysize;
	if (rem != 0)
		num_blocks++;
	blocks = make_keysize_blocks(enciphered, keysize);
	trans_blocks = transpose_blocks(enciphered, blocks, keysize);

	for (i = 0; i < keysize; i++) {
		key->data[i] = find_singlebyte_cipher(trans_blocks[i], 1);
	}

	for (i = 0; i < num_blocks; i++)
		bytes_put(blocks[i]);
	for (i = 0; i < keysize; i++)
		bytes_put(trans_blocks[i]);
	free(blocks);
	free(trans_blocks);

	return key;
}

struct bytes *decipher_repeatingxor(struct bytes *enciphered)
{
	int i, len, highest_index;
	int *pot_keysizes = NULL;
	double highscore = 0.0;
	struct bytes **pot_keys = NULL;
	struct bytes *deciphered = NULL;
	i = len = highest_index = 0;
	find_pot_keysizes(&pot_keysizes, &len, enciphered);
	pot_keys = malloc(len * sizeof(*pot_keys));

	for (i = 0; i < len; i++) {
		pot_keys[i] = guess_key(enciphered, pot_keysizes[i]);
	}
	for (i = 0; i < len; i++) {
		double score = score_repeatingxor(enciphered, pot_keys[i]);
		if (score > highscore) {
			highscore = score;
			highest_index = i;
		}
	}

	deciphered = xor_repeating_key(enciphered, pot_keys[highest_index]);

	for (i = 0; i < len; i++)
		bytes_put(pot_keys[i]);

	free(pot_keysizes);
	free(pot_keys);

	return deciphered;
}

void decipher_repeatingxor_file(char *deciphered, char *fname)
{
}

void free_cryptlib(void *item)
{
	free(item);
}
