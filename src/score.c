#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef S_SPLINT_S
#include <ctype.h>
#endif

#include "score.h"
#include "dataconvert.h"
#include "datamanip.h"
#include "bytes-node.h"

static double calc_wordsscore(struct bytes_node *tokens);
static double calc_letterscore(struct bytes *bytes);
static double calc_letterfreqscore(unsigned char byte);
static double calc_startwordscore(struct bytes *bytes);
static double calc_endwordscore(struct bytes *bytes);
static void score_bytes(double *scores, struct bytes *bytes);
static double calc_bytescore(struct bytes *bytes);

void score_bytes(double *scores, struct bytes *bytes)
{
	int i = 0;
	unsigned char c = 0;
	struct bytes *xored = NULL;
	xored = bytes_create(bytes->len);
	assert(xored != NULL);

	for (i = 0, c = START_CIPHER; i < CIPHERS; i++, c++) {
		xorbytes(xored, bytes, c);
		scores[i] = calc_bytescore(xored);
	}

	bytes_put(xored);
}

double calc_bytescore(struct bytes *bytes)
{
	double score = 0.0;
	struct bytes_node *tokens = NULL;

	tokens = bytes_node_init_as_tokens(bytes, SEPARATORS);
	score += calc_letterscore(bytes);
	score += calc_wordsscore(tokens);

	bytes_node_put(tokens);

	return score;
}

static double calc_wordsscore(struct bytes_node *tokens)
{
	int i = 0;
	double score = 0.0;

	for (i = 0; i < bytes_node_len(tokens); i++) {
		score += calc_startwordscore(
				bytes_node_element_at(tokens, i)->bytes);
		score += calc_endwordscore(
				bytes_node_element_at(tokens, i)->bytes);
	}

	return score;
}

static double calc_letterscore(struct bytes *bytes)
{
	int i = 0;
	double score = 0.0;

	for (i = 0; i < bytes->len; i++) {
		score += calc_letterfreqscore(bytes->data[i]);
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

static double calc_startwordscore(struct bytes *bytes)
{
	switch(toupper(bytetoascii(bytes->data[0]))) {
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

static double calc_endwordscore(struct bytes *bytes)
{
	switch(toupper(bytetoascii(bytes->data[bytes->len - 1]))) {
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

struct bytes *highest_scoring_xor(struct bytes *bytes)
{
	int i = 0;
	unsigned char cipher = 0;
	struct bytes *xored = bytes_create(bytes->len);
	double highscore = 0.0;
	double *scores = malloc(CIPHERS * sizeof(*scores));
	score_bytes(scores, bytes);

	for (i = 0; i < CIPHERS; i++) {
		if (scores[i] > highscore) {
			highscore = scores[i];
			assert(i >= 0x00 && i <= 0xFF);
			cipher = START_CIPHER + (unsigned char)i;
               }
	}
      	i = cipher;
      	xorbytes(xored, bytes, cipher);

	free(scores);

	return xored;
}
