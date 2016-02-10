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

/*
 *void encipher_repeatingxor(char *enciphered, char *str, char *key)
 *{
 *        int i, b, len, key_len;
 *        unsigned char *bytes, *key_bytes;
 *        key_len = len = b = i = 0;
 *        bytes = key_bytes = NULL;
 *
 *        bytes = malloc(strlen(str) * sizeof(bytes[0]));
 *        key_bytes = malloc(strlen(key) * sizeof(key_bytes[0]));
 *        assert(bytes != NULL);
 *        assert(key_bytes != NULL);
 *        len = strtobytes(bytes, str);
 *        key_len = strtobytes(key_bytes, key);
 *
 *        printf("Input: %s", str);
 *
 *        printf("XORd: ");
 *        for (b = i = 0; i < len; i++, b++) {
 *                unsigned char temp = 0;
 *                temp = xorbyte(bytes[i], key_bytes[i % key_len]);
 *                if (enciphered[b] == '\0') {
 *                        printf("found end");
 *                        break;
 *                }
 *                if (temp < 16) {
 *                        enciphered[b++] = 0;
 *                }
 *                enciphered[b] = bytetoascii(temp);
 *        }
 *
 *        printf("\n");
 *}
 *
 *void decipher_repeatingxor(char *deciphered, char *enciphered, char *key)
 *{
 *        int i, len, key_len;
 *        unsigned char *bytes, *key_bytes;
 *        len = key_len = i = 0;
 *        bytes = malloc(strlen(enciphered) * sizeof(bytes[0]));
 *        key_bytes = malloc(strlen(key) * sizeof(key_bytes[0]));
 *        assert(bytes != NULL);
 *        assert(key_bytes != NULL);
 *        len = hexstrtobytes(bytes, enciphered);
 *        key_len = strtobytes(key_bytes, key);
 *        assert(len > 0);
 *        assert(key_len > 0);
 *        for (i = 0; i < len; i++) {
 *                unsigned char xoredbyte = xorbyte(bytes[i],
 *                                key_bytes[i % key_len]);
 *                deciphered[i] = bytetoascii(xoredbyte);
 *        }
 *}
 */
