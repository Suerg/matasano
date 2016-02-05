#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef S_SPLINT_S
#include <ctype.h>
#endif

#include "score.h"
#include "dataconvert.h"
#include "datamanip.h"

static double calc_wordsscore(unsigned char **toks, int len, int *tok_lens);
static double calc_letterscore(unsigned char *bytes, int len);
static double calc_letterfreqscore(unsigned char byte);
static double calc_startwordscore(unsigned char *bytes);
static double calc_endwordscore(unsigned char *bytes, int len);

void score_bytes(double *scores, unsigned char *bytes, int len)
{
	int i = 0;
	unsigned char c = 0;
	unsigned char *xored = NULL;
	xored = malloc(len * sizeof(xored[0]));
	assert(xored != NULL);

	for (i = 0, c = START_CIPHER; i < CIPHERS; i++, c++) {
		xorbytes(xored, bytes, len, c);
		scores[i] = calc_bytescore(xored, len);
	}

	free(xored);
}

double calc_bytescore(unsigned char *bytes, int len)
{
	int i, tok_len, *tok_lens = NULL;
	double score = 0.0;
	unsigned char *pby = NULL;
	unsigned char **toks = NULL;
	i = tok_len = 0;

	toks = malloc(len * sizeof(toks[0]));
	tok_lens = malloc(len * sizeof(tok_lens[0]));
	assert(toks != NULL);
	assert(tok_lens != NULL);

	for (i = 0; i < len; i++) {
		toks[i] = NULL;
		toks[i] = malloc(len * sizeof(toks[i][0]));
		assert(toks[i] != NULL);
	}

	pby = bytetok(bytes, SEPARATORS, len, &tok_len);
	for (i = 0; pby != NULL; i++) {
		if (i >= len)
			break;
		tok_lens[i] = tok_len;
		memcpy(toks[i], pby, tok_len * sizeof(toks[i][0]));
		pby = bytetok(bytes, SEPARATORS, len, &tok_len);
	}

	score += calc_letterscore(bytes, len);
	score += calc_wordsscore(toks, i - 1, tok_lens);

	for (i = 0; i < len; i++) {
		free(toks[i]);
	}
	free(toks);
	free(tok_lens);

	return score;
}

static double calc_wordsscore(unsigned char **toks, int len, int *tok_lens)
{
	int i = 0;
	double score = 0.0;

	for (i = 0; i < len; i++) {
		score += calc_startwordscore(toks[i]);
		score += calc_endwordscore(toks[i], tok_lens[i]);
	}

	return score;
}

static double calc_letterscore(unsigned char *bytes, int len)
{
	int i = 0;
	double score = 0.0;

	for (i = 0; i < len; i++) {
		score += calc_letterfreqscore(bytes[i]);
	}

	return score;
}

static double calc_letterfreqscore(unsigned char byte)
{
	switch(toupper(bytetoascii(byte))) {
	case 'E':
		return 12.702;
	case 'T':
		return 9.056;
	case 'A':
		return 8.167;
	case 'O':
		return 7.507;
	case 'I':
		return 6.966;
	case 'N':
		return 6.749;
	case 'S':
		return 6.397;
	case 'H':
		return 6.094;
	case 'R':
		return 5.987;
	case 'D':
		return 4.253;
	case 'C':
		return 2.782;
	case 'U':
		return 2.758;
	case 'M':
		return 2.406;
	case 'W':
		return 2.361;
	case 'F':
		return 2.228;
	case 'G':
		return 2.015;
	case 'Y':
		return 1.974;
	case 'P':
		return 1.929;
	case 'B':
		return 1.492;
	case 'V':
		return 0.978;
	case 'K':
		return 0.772;
	case 'J':
		return 0.153;
	case 'X':
		return 0.150;
	case 'Q':
		return 0.095;
	case 'Z':
		return 0.074;
	default:
		return 0.000;
	}
}

static double calc_startwordscore(unsigned char *bytes)
{
	switch(toupper(bytetoascii(bytes[0]))) {
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
			return 0.000;
	}
}

static double calc_endwordscore(unsigned char *bytes, int len)
{
	switch(toupper(bytetoascii(bytes[len - 1]))) {
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
			return 0.001;
		case 'J':
			return 0.003;
		default:
			return 0.000;
	}
}
