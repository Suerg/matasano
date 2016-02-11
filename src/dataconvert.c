/*
 * Author: David Piper
 * cryptlib for matasano cryptopals.com challenges
 */
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

struct bytes *hextobase64(struct bytes *hex)
{
	int i, j, base64len;
	struct bytes *base64 = NULL;
	i = j = 0;

	base64len = (int)((hex->len * 2) / 1.5);

	if ((hex->len * 2) % 2 != 0)
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

unsigned char *bytetok(unsigned char *bytes, int len, char *delimiters,
		int offset,
		int *tok_len, int *delim_len)
{
	int end_i, i, j, contains, remaining_len;
	unsigned char *bytes_start_from = NULL;
	unsigned char *start = NULL;
	end_i = i = j = remaining_len = contains = 0;

	if (offset < 0 || len <= 0 || offset >= len)
		return start;

	bytes_start_from = bytes + offset;
	remaining_len = len - offset;

	for (i = 0; i < remaining_len; i++) {
		contains = 0;
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes_start_from[i] == (int)delimiters[j])
				contains = 1;
		if (!contains) {
			start = &bytes_start_from[i];
			break;
		}
	}

	if (!start)
		return start;

	for (i = start - bytes_start_from; i < remaining_len; i++) {
		contains = 0;
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes_start_from[i] == (int)delimiters[j])
				contains = 1;
		if (contains) {
			end_i = i - 1;
			break;
		} else if (i == remaining_len - 1) {
			end_i = i;
		}
	}

	for (i = end_i + 1; i < remaining_len; i++) {
		contains = 0;
		for (j = 0; j < (int)strlen(delimiters); j++)
			if ((int)bytes_start_from[i] == (int)delimiters[j])
				contains = 1;
		if (!contains) {
			*delim_len = (i - end_i) - 1;
			break;
		}
	}

	*tok_len = (&bytes_start_from[end_i] - start) + 1;

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

int printable(struct bytes *bytes)
{
	int res = 1, i = 0;

	for (i = 0; i < bytes->len; i++)
		if (bytes->data[i] > 127)
			res = 0;

	return res;
}


void bytestostr(char *str, struct bytes *bytes)
{
	int i = 0;

	for (i = 0; i < bytes->len; i++)
		str[i] = bytetoascii(bytes->data[i]);

	str[bytes->len] = '\0';
}

void bytestohexstr(char *hexstr, struct bytes *bytes, int uppercase)
{
	int i, j;
	char d1, d2;
	d1 = d2 = 0;
	i = j = 0;

	for (i = 0; i < bytes->len; i++) {
		bytetohexs(&d1, &d2, bytes->data[i], uppercase);
		hexstr[j++] = d1;
		hexstr[j++] = d2;
	}
}

void bytetohexs(char *d1, char *d2, unsigned char byte, int uppercase)
{
	char n1 = (char)(byte >> 4) & 0xF;
	char n2 = (char)(byte & 0xF);

	*d1 = nibbletohexdigit(n1, uppercase);
	*d2 = nibbletohexdigit(n2, uppercase);
}

char nibbletohexdigit(char nibble, int uppercase)
{
	if (nibble >= 0 && nibble <= 9)
		return '0' + nibble;
	switch(nibble) {
	case 10:
		if (uppercase)
			return 'A';
		else
			return 'a';
	case 11:
		if (uppercase)
			return 'B';
		else
			return 'b';
	case 12:
		if (uppercase)
			return 'C';
		else
			return 'c';
	case 13:
		if (uppercase)
			return 'D';
		else
			return 'd';
	case 14:
		if (uppercase)
			return 'E';
		else
			return 'e';
	case 15:
		if (uppercase)
			return 'F';
		else
			return 'f';
	default:
		return 0;
	}
}
