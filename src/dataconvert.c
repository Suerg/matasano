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

int strtobytes(unsigned char *bytes, char *str)
{
	int i = 0;
	for (i = 0; i < (int)strlen(str); i++) {
		assert(str[i] >= 0);
		bytes[i] = (unsigned char)str[i];
	}
	return i;
}

unsigned char hexdigittobyte(char hexdigit)
{
	assert((hexdigit >= 0x0 && hexdigit <= 0x9)
		|| (hexdigit >= 'A' && hexdigit <= 'F')
		|| (hexdigit >= 'a' && hexdigit <= 'f'));
	if (hexdigit <= 0x9)
		return (unsigned char)hexdigit;
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

void bytecpy(unsigned char *copy, unsigned char *bytes, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
		copy[i] = bytes[i];
}

void xortwo(unsigned char *xored, unsigned char *buf1,
		unsigned char *buf2, int num)
{
	int i;
	for (i = 0; i < num; i++)
		xored[i] = buf1[i] ^ buf2[i];
}

void hexstrtonibbles(unsigned char *nibbles, char *hexstr)
{
	int i;

	for (i = 0; i < (int) strlen(hexstr); i++) {
		nibbles[i] = hexdigittobyte(hexstr[i]);
	}
}

int nibblestobytes(unsigned char *bytes, unsigned char *nibbles, int num)
{
	int i, b;
	unsigned char n1, n2;
	n1 = n2 = 0x0;
	i = b = 0;
	do {
		if (i == 0 && num % 2 != 0) {
			n1 = 0;
			n2 = nibbles[i];
		} else {
			n1 = nibbles[i];
			n2 = nibbles[i + 1];
		}
		bytes[b] = nibblestobyte(n1, n2);
		i += 2;
		b++;
	} while(i < num - 1);
	return b;
}

int hexstrtobytes(unsigned char *bytes, char *hexstr)
{
	int num;
	unsigned char *nibbles = NULL;

	assert(strlen(hexstr) > 0);
	assert(bytes != NULL);

	nibbles = malloc(strlen(hexstr) * sizeof(nibbles[0]));
	assert(nibbles != NULL);
	hexstrtonibbles(nibbles, hexstr);

	num = nibblestobytes(bytes, nibbles, (int)strlen(hexstr));
	free(nibbles);
	return num;
}

unsigned char nibblestobyte(unsigned char n1, unsigned char n2)
{
	return n1 << 4 | n2;
}

int hextobase64(char *base64, char *hex, int bytes)
{
	int i, j;

	for (j = i = 0; i < bytes - 2; i += 3) {
		base64[j++] = (hex[i] & 0xFC) >> 2;
		base64[j++] = ((hex[i] & 0x03) << 4)
				| ((hex[i + 1] & 0xF0) >> 4);
		base64[j++] = ((hex[i + 1] & 0x0F) << 2)
				| ((hex[i + 2] & 0xC0) >> 6);
		base64[j++] = hex[i + 2] & 0x3F;
	}

	return j;
}

void base64tostr(char *str, char *base64, int digits)
{
	int i;

	for (i = 0; i < digits; i++) {
		str[i] = base64todigit(asciitobyte(base64[i]));
	}
	str[i] = '\0';
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

unsigned char xorbyte(unsigned char byte, unsigned char xorwith)
{
	return byte ^ xorwith;
}

void xorbytes(unsigned char *xored, unsigned char *bytes,
		int num, unsigned char xorwith)
{
	int i;

	for (i = 0; i < num; i++)
		xored[i] = xorbyte(bytes[i], xorwith);
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

char bytetoascii(unsigned char byte)
{
	if (byte > 127)
		exit(1);
	return (char)byte;
}

unsigned char asciitobyte(char ascii)
{
	if (ascii < 0)
		exit(1);
	return (unsigned char)ascii;
}
