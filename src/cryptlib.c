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
#include "cryptlib.h"

#define BUF_SZ 4

/*
 * Takes a c-string containing hex digits
 * produces a deciphered xor version.
 * Assumes it was enciphered with a single-byte xor cipher.
 */
double decipherxor(char *deciphered, char *hexstr)
{
	unsigned char *xored, *bytes, cipher = 0x00;
	double highscore = 0.0, *scores = NULL;
	int i, numbytes = 0;
	xored = bytes = NULL;

	bytes = malloc(strlen(hexstr) * sizeof(bytes[0]));
	assert(bytes != NULL);

	numbytes = hexstrtobytes(bytes, hexstr);
	xored = malloc(numbytes * sizeof(xored[0]));

	scores = malloc((size_t) CIPHERS * sizeof(scores[0]));
	assert(scores != NULL);

	score_bytes(scores, bytes, numbytes);

	for (i = 0; i < CIPHERS; i++) {
		if (scores[i] > highscore) {
			highscore = scores[i];
			assert(i >= 0x00 && i <= 0xFF);
			cipher = START_CIPHER + (unsigned char)i;
		}
	}
	i = cipher;
	xorbytes(xored, bytes, numbytes, cipher);

	for (i = 0; i < numbytes; i++) {
		/*
		 * xored should be within ascii/printable range now.
		 * if not something went wrong
		 */
		deciphered[i] = bytetoascii(xored[i]);
	}

	free(scores);
	free(bytes);
	free(xored);

	return  highscore;
}

unsigned char * bytetok(unsigned char *bytes, char *delimiters,
		int len, int *tok_len)
{
	int i, j, contains;
	unsigned char *start = NULL;
	i = j = len = contains = 0;

	for (i = 0; i < len; i++) {
		for (j = 0; j < (int) strlen(delimiters); j++)
			if ((int) bytes[i] == (int) delimiters[j])
				contains = 1;
		if (!contains)
			start = &bytes[i];
	}
	contains = 0;
	for (i = 0; i < len; i++) {
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes[i] == (int)delimiters[j])
				contains = 1;
		if (contains)
			break;
	}
	*tok_len = i;
	return start;
}

void encipher_repeatingxor(char *enciphered, char *str, char *key)
{
	int i, b, len, key_len;
	unsigned char *bytes, *key_bytes;
	key_len = len = b = i = 0;
	bytes = key_bytes = NULL;

	bytes = malloc(strlen(str) * sizeof(bytes[0]));
	key_bytes = malloc(strlen(key) * sizeof(key_bytes[0]));
	assert(bytes != NULL);
	assert(key_bytes != NULL);
	len = strtobytes(bytes, str);
	key_len = strtobytes(key_bytes, key);

	printf("Input: %s", str);

	printf("XORd: ");
	for (b = i = 0; i < len; i++, b++) {
		unsigned char temp = 0;
		temp = xorbyte(bytes[i], key_bytes[i % key_len]);
		if (enciphered[b] == '\0') {
			printf("found end");
			break;
		}
		if (temp < 16) {
			enciphered[b++] = 0;
		}
		enciphered[b] = bytetoascii(temp);
	}

	printf("\n");
}

void decipher_repeatingxor(char *deciphered, char *enciphered, char *key)
{
	int i, len, key_len;
	unsigned char *bytes, *key_bytes;
	len = key_len = i = 0;
	bytes = malloc(strlen(enciphered) * sizeof(bytes[0]));
	key_bytes = malloc(strlen(key) * sizeof(key_bytes[0]));
	assert(bytes != NULL);
	assert(key_bytes != NULL);
	len = hexstrtobytes(bytes, enciphered);
	key_len = strtobytes(key_bytes, key);
	assert(len > 0);
	assert(key_len > 0);
	for (i = 0; i < len; i++) {
		unsigned char xoredbyte = xorbyte(bytes[i],
				key_bytes[i % key_len]);
		deciphered[i] = bytetoascii(xoredbyte);
	}
}
