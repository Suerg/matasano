#include <stdio.h>
#include <stddef.h>
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
static void score_bytes(double *scores, struct bytes *bytes,
		int include_words);
static double calc_bytescore(struct bytes *bytes, int include_words);
static double calc_digraphs(struct bytes *bytes);
static double calc_digraphscore(unsigned char b1, unsigned char b2);
static void remove_newline(char *line);
static char *find_newline(char *line);


void score_bytes(double *scores, struct bytes *bytes, int include_words)
{
	int i = 0;
	unsigned char c = 0;
	struct bytes *xored = NULL;
	xored = bytes_create(bytes->len);
	assert(xored != NULL);

	for (i = 0, c = START_CIPHER; i < (int)CIPHERS; i++, c++) {
		xorbytes(xored, bytes, c);
		scores[i] = calc_bytescore(xored, include_words);
	}

	bytes_put(xored);
}

static double calc_digraphscore(unsigned char b1, unsigned char b2)
{
	char c1 = toupper(bytetoascii(b1));
	char c2 = toupper(bytetoascii(b2));

	if (c1 == 'T' && c2 == 'H')
		return 4.21;
	else if (c1 == 'H' && c2 == 'E')
		return 3.80;
	else if (c1 == 'I' && c2 == 'N')
		return 2.42;
	else if (c1 == 'A' && c2 == 'N')
		return 2.19;
	else if (c1 == 'E' && c2 == 'R')
		return 2.11;
	else if (c1 == 'R' && c2 == 'E')
		return 1.88;
	else if (c1 == 'N' && c2 == 'D')
		return 1.63;
	else if (c1 == 'O' && c2 == 'N')
		return 1.52;
	else if (c1 == 'E' && c2 == 'N')
		return 1.39;
	else if (c1 == 'A' && c2 == 'T')
		return 1.39;
	else if (c1 == 'E' && c2 == 'S')
		return 1.30;
	else if (c1 == 'E' && c2 == 'D')
		return 1.25;
	else if (c1 == 'O' && c2 == 'R')
		return 1.21;
	else if (c1 == 'O' && c2 == 'F')
		return 1.20;
	else if (c1 == 'A' && c2 == 'R')
		return 1.16;
	else if (c1 == 'I' && c2 == 'S')
		return 1.16;
	else if (c1 == 'I' && c2 == 'T')
		return 1.14;
	else if (c1 == 'O' && c2 == 'U')
		return 1.13;
	else if (c1 == 'T' && c2 == 'O')
		return 1.13;
	else if (c1 == 'H' && c2 == 'A')
		return 1.10;
	else if (c1 == 'N' && c2 == 'G')
		return 1.09;
	else if (c1 == 'S' && c2 == 'T')
		return 1.08;
	else if (c1 == 'T' && c2 == 'E')
		return 1.07;
	else if (c1 == 'A' && c2 == 'S')
		return 1.00;
	else if (c1 == 'H' && c2 == 'I')
		return 0.99;
	else if (c1 == 'S' && c2 == 'E')
		return 0.94;
	else
		return 0.00;
}

static double calc_digraphs(struct bytes *bytes)
{
	int i = 0;
	double score = 0.0;

	for (i = 0; i < bytes->len - 1; i++) {
		score += calc_digraphscore(bytes->data[i], bytes->data[i + 1]);
	}

	return score;
}

static double calc_bytescore(struct bytes *bytes, int include_words)
{
	double score = 0.0;
	struct bytes_node *tokens = NULL;

	if (printable(bytes)) {
		tokens = bytes_node_init_as_tokens(bytes, SEPARATORS);
		score += calc_letterscore(bytes);
		if (include_words)
			score += calc_wordsscore(tokens);
		score += calc_digraphs(bytes);
		bytes_node_put(tokens);
	}

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

	for (i = 0; i < bytes->len; i++)
		score += calc_letterfreqscore(bytes->data[i]);

	return score;
}

static double calc_letterfreqscore(unsigned char byte)
{
	switch(toupper(bytetoascii(byte))) {
	case ' ':
		return 18.288;
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

unsigned char find_singlebyte_cipher(struct bytes *bytes, int include_words)
{
	int i = 0;
	unsigned char cipher = 0;
	double highscore = 0.0;
	double *scores = malloc(CIPHERS * sizeof(*scores));
	score_bytes(scores, bytes, include_words);

	for (i = 0; i < (int)CIPHERS; i++) {
		if (scores[i] > highscore) {
			highscore = scores[i];
			assert(i >= 0x00 && i <= 0xFF);
			cipher = START_CIPHER + (unsigned char)i;
               }
	}

	free(scores);

	return cipher;
}

struct bytes *highest_scoring_xor(struct bytes *bytes)
{
	unsigned char cipher = 0;
	struct bytes *xored = bytes_create(bytes->len);
      	cipher = find_singlebyte_cipher(bytes, 1);
	xorbytes(xored, bytes, cipher);

	return xored;
}

static void remove_newline(char *line)
{
	char *newline = find_newline(line);
	if (newline != NULL)
		*newline = '\0';
}

static char *find_newline(char *line)
{
	return strchr(line, '\n');
}

struct bytes *highest_scoring_xor_in_file(FILE *fp)
{
	char *line;
	struct bytes *highest = NULL;
	double highscore = 0.0;
	int line_len = 600;
	assert(fp != NULL);

	line = malloc(line_len * sizeof(*line) + 1);

	while (fgets(line, line_len, fp) != NULL) {
		struct bytes *line_bytes = NULL;
		struct bytes *highest_in_line = NULL;
		double line_score = 0.0;

		remove_newline(line);
		line_bytes = bytes_init_from_hexstr(line);
		highest_in_line = highest_scoring_xor(line_bytes);
		line_score = calc_bytescore(highest_in_line, 1);

		if (line_score > highscore) {
			highscore = line_score;
			highest_in_line = bytes_get(highest_in_line);
			if (highest != NULL)
				bytes_put(highest);
			highest = highest_in_line;
		}

		bytes_put(highest_in_line);
		bytes_put(line_bytes);
	}

	free(line);

	return highest;
}

double score_repeatingxor(struct bytes *bytes, struct bytes *key)
{
	struct bytes *xored = xor_repeating_key(bytes, key);
	double score = calc_bytescore(xored, 1);

	bytes_put(xored);
	return score;
}
