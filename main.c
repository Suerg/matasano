/*
 * Author: David Piper
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cryptlib.h"

#define BUF_SZ 4

void xorAZ(char **xored, char *bytes, int numbytes);
int readstr(char **str);
int remove_newline(char *str);
char* find_newline(char *str);
void challenge3(void);
void challenge4(void);

int main(void)
{
	challenge4();

	return 0;
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
		remove_newline(line);
		/*printf("Line %d: ", ++i);*/
		score = decipherxor(deciphered, line);
		/*printf("%s", deciphered);*/
		/*printf("Score: %f", score);*/

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
		if (remove_newline(*str))
			break;
	}
	return (int) strlen(*str);
}

char* find_newline(char *str)
{
	return strchr(str, '\n');
}

/*
 * Replaces a newline char in str with \0.
 * Returns int indicating if removal occurred.
 */
int remove_newline(char *str)
{
	char *newline = NULL;
	newline = find_newline(str);
	if (newline != NULL) {
		*newline = '\0';
		return 1;
	} else {
		return 0;
	}
}
