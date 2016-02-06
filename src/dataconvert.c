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

#include "dataconvert.h"

unsigned char hexdigittobyte(char hexdigit)
{
	assert((hexdigit >= '0' && hexdigit <= '9')
		|| (hexdigit >= 'A' && hexdigit <= 'F')
		|| (hexdigit >= 'a' && hexdigit <= 'f'));
	if (hexdigit >= '0' && hexdigit <= '9')
		return asciitobyte(hexdigit - '0');
	switch (toupper(hexdigit)) {
	case 'A':
		return 10;
		break;
	case 'B':
		return 11;
		break;
	case 'C':
		return 12;
		break;
	case 'D':
		return 13;
		break;
	case 'E':
		return 14;
		break;
	case 'F':
		return 15;
		break;
	default:
		exit(1);
	}
}

/*
 *void xortwo(unsigned char *xored, unsigned char *b1,
 *                unsigned char *b2, int len)
 *{
 *        int i;
 *        for (i = 0; i < num; i++)
 *                xored[i] = buf1[i] ^ buf2[i];
 *}
 */

struct bytes *hextobase64(struct bytes *hex)
{
	int i, j, base64len;
	struct bytes *base64 = NULL;
	i = j = 0;

	base64len = (int)(hex->len / 1.5);

	if (hex->len % 2 != 0)
		base64len++;
	base64 = bytes_create(base64len);

	for (j = i = 0; i < hex->len - 2; i += 3) {
		base64->data[j++] = (hex->data[i] & 0xFC) >> 2;
		base64->data[j++] = ((hex->data[i] & 0x03) << 4)
				| ((hex->data[i + 1] & 0xF0) >> 4);
		base64->data[j++] = ((hex->data[i + 1] & 0x0F) << 2)
				| ((hex->data[i + 2] & 0xC0) >> 6);
		base64->data[j++] = hex->data[i + 2] & 0x3F;
	}

	return base64;
}

/*
 *void base64tostr(char *str, struct bytes *base64)
 *{
 *        int i;
 *
 *        for (i = 0; i < base64->len; i++) {
 *                str[i] = base64todigit(asciitobyte(base64[i]));
 *        }
 *        str[i] = '\0';
 *}
 */

char base64todigit(unsigned char value)
{
	if (value <= 25)
		return bytetoascii(value % 26 + 'A');
	else if (value >= 26 && value <= 51)
		return bytetoascii(value % 26 + 'a');
	else if (value >= 52 && value <= 61)
		return bytetoascii((value - 52) % 10 + '0');
	else if (value == 62)
		return '+';
	else if (value == 63)
		return '/';
	else
		return '=';
}

/*
 *unsigned char xorbyte(unsigned char byte, unsigned char xorwith)
 *{
 *        return byte ^ xorwith;
 *}
 *
 *void xorbytes(unsigned char *xored, unsigned char *bytes,
 *                int num, unsigned char xorwith)
 *{
 *        int i;
 *
 *        for (i = 0; i < num; i++)
 *                xored[i] = xorbyte(bytes[i], xorwith);
 *}
 */

unsigned char *bytetok(unsigned char *bytes, int len, char *delimiters,
		       int *tok_len)
{
	int i, j, contains;
	unsigned char *start = NULL;
	i = j = contains = 0;

	for (i = 0; i < len; i++) {
		contains = 0;
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes[i] == (int)delimiters[j])
				contains = 1;
		if (!contains) {
			start = &bytes[i];
			break;
		}
	}

	contains = 0;
	for (i = start - bytes; i < len; i++) {
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes[i] == (int)delimiters[j])
				contains = 1;
		if (contains)
			break;
	}
	*tok_len = (&bytes[i - 1] - start) + 1;

	return start;
}

char bytetoascii(unsigned char byte)
{
	assert(byte <= 127);
	return (char)byte;
}

unsigned char asciitobyte(char ascii)
{
	assert(ascii >= 0);
	return (unsigned char)ascii;
}
