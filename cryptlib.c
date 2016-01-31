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

#include "cryptlib.h"

#define BUF_SZ 4

void xortwo(char *xored, char *buf1, char *buf2, int num)
{
	int i;
	for (i = 0; i < num; i++)
		xored[i] = buf1[i] ^ buf2[i];
}

void strtohex_nibbles(char *nibbles, char *str)
{
	int i;
	char digit[4];
	digit[1] = '\0';

	for (i = 0; i < (int) strlen(str); i++) {
		digit[0] = str[i];
		nibbles[i] = (char) strtol(digit, NULL, 16);
	}
}

int nibblestobytes(char *bytes, char *nibbles, int num)
{
	int i, b;
	char n1, n2;
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

int strtohex_bytes(char *bytes, char *str)
{
	int num;
	char *nibbles = NULL;
	nibbles = malloc(strlen(str) * sizeof(nibbles[0]));
	assert(nibbles != NULL);
	strtohex_nibbles(nibbles, str);

	num = nibblestobytes(bytes, nibbles, (int) strlen(str));
	free(nibbles);
	return num;
}

char nibblestobyte(char n1, char n2)
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
		str[i] = base64todigit(base64[i]);
	}
	str[i] = '\0';
}

char base64todigit(char value)
{
	if (value <= 25)
		return value % 26 + 'A';
	else if (value >= 26 && value <= 51)
		return value % 26 + 'a';
	else if (value >= 52 && value <= 61)
		return (value - 52) % 10 + '0';
	else if (value == 62)
		return '+';
	else if (value == 63)
		return '/';
	else
		return '=';
}

char xorbyte(char byte, char xorwith)
{
	return byte ^ xorwith;
}

void xorbytes(char *xored, char *bytes, int num, char xorwith)
{
	int i;

	for (i = 0; i < num; i++)
		xored[i] = xorbyte(bytes[i], xorwith);
}

void scorebytes(double *scores, char *bytes, int num)
{
	int i;
	char c;
	char *xored = NULL;
	xored = malloc(num + 1 * sizeof(xored[0]));
	assert(xored != NULL);
	xored[num] = '\0';

	for (i = 0, c = START_CIPHER; i < CIPHERS; i++, c++) {
		xorbytes(xored, bytes, num, c);
		scores[i] = scorechars(xored);
	}

	free(xored);
}

double score_start(char *word)
{
	switch (toupper(word[0])) {
	case 'T':
		return 16.708;
	case 'A':
		return 11.670;
	case 'O':
		return 7.529;
	case 'S':
		return 6.987;
	case 'I':
		return 6.953;
	case 'W':
		return 6.563;
	case 'H':
		return 5.447;
	case 'B':
		return 4.715;
	case 'C':
		return 4.301;
	case 'F':
		return 3.991;
	case 'M':
		return 3.772;
	case 'P':
		return 3.510;
	case 'D':
		return 2.988;
	case 'L':
		return 2.552;
	case 'R':
		return 2.274;
	case 'E':
		return 2.257;
	case 'N':
		return 2.241;
	case 'G':
		return 1.898;
	case 'U':
		return 1.055;
	case 'V':
		return 0.743;
	case 'Y':
		return 0.720;
	case 'K':
		return 0.570;
	case 'J':
		return 0.365;
	case 'Q':
		return 0.208;
	case 'X':
		return 0.020;
	case 'Z':
		return 0.014;
	default:
		return 0.0;
	}
}

double score_end(char *word)
{
	switch(toupper(word[strlen(word) - 1])) {
	case 'E':
		return 20.660;
	case 'S':
		return 12.826;
	case 'D':
		return 10.927;
	case 'T':
		return 9.253;
	case 'N':
		return 8.343;
	case 'Y':
		return 6.003;
	case 'R':
		return 5.838;
	case 'F':
		return 4.449;
	case 'O':
		return 4.430;
	case 'H':
		return 2.999;
	case 'G':
		return 2.978;
	case 'A':
		return 2.797;
	case 'L':
		return 2.755;
	case 'M':
		return 1.747;
	case 'W':
		return 0.921;
	case 'K':
		return 0.891;
	case 'I':
		return 0.818;
	case 'P':
		return 0.529;
	case 'U':
		return 0.382;
	case 'C':
		return 0.251;
	case 'X':
		return 0.096;
	case 'B':
		return 0.073;
	case 'V':
		return 0.019;
	case 'Z':
		return 0.011;
	case 'J':
		return 0.003;
	default:
		return 0.0;
	}
}

double score_letterfreq(char *chars)
{
	double score = 0.0;

	score += 12.702 * countc(chars, 'E');
	score += 9.056 * countc(chars, 'T');
	score += 8.167 * countc(chars, 'A');
	score += 7.507 * countc(chars, 'O');
	score += 6.966 * countc(chars, 'I');
	score += 6.749 * countc(chars, 'N');
	score += 6.327 * countc(chars, 'S');
	score += 6.094 * countc(chars, 'H');
	score += 5.987 * countc(chars, 'R');
	score += 4.253 * countc(chars, 'D');
	score += 2.782 * countc(chars, 'C');
	score += 2.758 * countc(chars, 'U');
	score += 2.406 * countc(chars, 'M');
	score += 2.361 * countc(chars, 'W');
	score += 2.228 * countc(chars, 'F');
	score += 2.015 * countc(chars, 'G');
	score += 1.974 * countc(chars, 'Y');
	score += 1.929 * countc(chars, 'P');
	score += 1.492 * countc(chars, 'B');
	score += 0.978 * countc(chars, 'V');
	score += 0.772 * countc(chars, 'K');
	score += 0.153 * countc(chars, 'J');
	score += 0.150 * countc(chars, 'X');
	score += 0.095 * countc(chars, 'Q');
	score += 0.074 * countc(chars, 'Z');

	return score;
}

double scorewords(char **words, int num)
{
	double score = 0.0;
	int i = 0;

	for (i = 0; i < num; i++) {
		score += score_start(words[i]);
		score += score_end(words[i]);
	}

	return score;
}

double scorechars(char *chars)
{
	int i = 0;
	double score = 0.0;
	char *pch, *copied;
	char **tokens = NULL;
	pch = copied = NULL;
	copied = malloc(strlen(chars) + 1 * sizeof(copied[0]));

	strcpy(copied, chars);
	tokens = malloc(strlen(chars) * sizeof(tokens[0]));
	assert(copied != NULL);
	assert(tokens != NULL);

	for (i = 0; i < (int) strlen(copied); i++) {
		tokens[i] = NULL;
		tokens[i] = malloc(strlen(chars) + 1 * sizeof(tokens[i][0]));
		assert(tokens[i] != NULL);
	}

	pch = strtok(copied, SEPARATORS);
	for(i = 0; pch != NULL; i++) {
		if (i >= (int)strlen(chars))
			break;
		strcpy(tokens[i], pch);
		pch = strtok(NULL, SEPARATORS);
	}

	score += score_letterfreq(chars);
	score += scorewords(tokens, i - 1);

	for (i = 0; i < (int)strlen(chars); i++) {
		free(tokens[i]);
	}
	free(tokens);
	free(copied);

	return score;
}

int countc(char *chars, char c)
{
	int i, count;
	char curr;
	i = count = 0;
	for (i = 0; i < (int) strlen(chars); i++) {
		curr = chars[i];
		if (toupper(curr) == c)
			count++;
	}
	return count;
}

double decipherxor(char *deciphered, char *hexstr)
{
	char *xored, *bytes, cipher;
	double highscore = 0.0, *scores = NULL;
	int i, numbytes = 0;
	xored = bytes = NULL;

	bytes = malloc(strlen(hexstr) * sizeof(bytes[0]));
	assert(bytes != NULL);

	numbytes = strtohex_bytes(bytes, hexstr);
	xored = malloc(numbytes * sizeof(xored[0]));

	scores = malloc((size_t) CIPHERS * sizeof(scores[0]));
	assert(scores != NULL);

	scorebytes(scores, bytes, numbytes);

	for (i = 0; i < CIPHERS; i++) {
		if (scores[i] > highscore) {
		highscore = scores[i];
			cipher = START_CIPHER + i;
		}
	}
	i = cipher;
	xorbytes(xored, bytes, numbytes, cipher);

	for (i = 0; i < numbytes; i++)
		deciphered[i] = xored[i];

	free(scores);
	free(bytes);
	free(xored);

	return  highscore;
}
