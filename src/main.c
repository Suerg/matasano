/*
 * Author: David Piper
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cryptlib.h"

#define BUF_SZ 4
#define TERM '~'

void xorAZ(char **xored, char *bytes, int numbytes);
int readstr(char **str);
int remove_term(char *str);
char* find_term(char *str);
void challenge3(void);
void challenge4(void);
void challenge5(void);
void challenge5_5(void);

int main(void)
{
	challenge5();

	return 0;
}

void challenge5_5(void)
{
	char *str, *key, *deciphered;
	int len;
	len = 0;
	str = key = deciphered =  NULL;

	printf("Enter the string to decipher: ");
	len = readstr(&str);
	assert(str != NULL);

	printf("Enter the repeating XOR key: ");
	readstr(&key);

	deciphered = malloc((len + 1) * sizeof(deciphered[0]));
	assert(deciphered != NULL);
	deciphered[len] = '\0';

	decipher_repeatingxor(deciphered, str, key);

	printf("Deciphered: %s\n", deciphered);

	free(deciphered);
	free(key);
	free(str);
}

void challenge3(void)
{
	double score = 0.0;
	char *deciphered, *hexstr;
	int len = 0;
	hexstr = deciphered = NULL;

	printf("Enter the hex str: ");
	len = readstr(&hexstr);
	assert(hexstr != NULL);

	deciphered = malloc(len * sizeof(deciphered[0]) + 1);
	assert(deciphered != NULL);
	deciphered[len] = '\0';

	score = decipherxor(deciphered, hexstr);

	printf("Deciphered: %s\n", deciphered);
	printf("Score: %f\n", score);

	free(deciphered);
	free(hexstr);
}

void challenge4(void)
{
	FILE *fp;
	int line_len = 500;
	double score, highscore;
	char *deciphered, *line, *decodedline;
	score = highscore = 0.0;
	deciphered = line = decodedline = NULL;

	line = malloc(line_len * sizeof(line[0]));
	decodedline = malloc(line_len * sizeof(decodedline[0]));
	deciphered = malloc(line_len * sizeof(deciphered[0]));
	deciphered[line_len - 1] = '\0';

	fp = fopen("res/4.txt", "r+");

	if (!fp)
		return;

	while (fgets(line, line_len, fp) != NULL) {
		remove_term(line);
		score = decipherxor(deciphered, line);

		if (score > highscore) {
			highscore = score;
			strcpy(decodedline, deciphered);
		}
	}

	printf("Decoded line: %s\n", decodedline);
	printf("Score: %f\n", highscore);

	fclose(fp);
	free(deciphered);
	free(decodedline);
	free(line);
}

void challenge5(void)
{
	char *str, *key, *enciphered, *deciphered;
	int i, len;
	i = len = 0;
	str = key = enciphered = deciphered =  NULL;

	printf("Enter the string to encipher: ");
	len = readstr(&str);
	assert(str != NULL);

	printf("Enter the repeating XOR key: ");
	readstr(&key);

	enciphered = malloc(1 + len  * sizeof(enciphered[0]));
	deciphered = malloc(1 + len  * sizeof(deciphered[0]));
	assert(enciphered != NULL);
	assert(deciphered != NULL);
	enciphered[len] = '\0';
	deciphered[len] = '\0';

	encipher_repeatingxor(enciphered, str, key);

	enciphered[len] = '\0';

	printf("Enciphered: ");
	for (i = 0; i < len; i++) {
		printf("%x", (unsigned int)enciphered[i]);
	}
	printf("\n");

	printf("Str len: %d\n", (int)strlen(enciphered));

	decipher_repeatingxor(deciphered, enciphered, key);

	printf("Deciphered: %s", deciphered);

	free(enciphered);
	free(key);
	free(str);
}


int readstr(char **str)
{
	char buffer[BUF_SZ];
	int size = BUF_SZ;
	*str = malloc(BUF_SZ * sizeof(*str[0]));
	strcpy(*str, "");
	while (fgets(buffer, BUF_SZ, stdin) != NULL) {
		size += 4;
		*str = realloc(*str, size * sizeof(*str[0]));
		strcat(*str, buffer);
		if (remove_term(*str))
			break;
	}
	return (int) strlen(*str);
}

char* find_term(char *str)
{
	return strchr(str, TERM);
}

/*
 * Replaces a term char in str with \0.
 * Returns int indicating if removal occurred.
 */
int remove_term(char *str)
{
	char *term = NULL;
	term = find_term(str);
	if (term != NULL) {
		*term = '\0';
		return 1;
	} else {
		return 0;
	}
}
