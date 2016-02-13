#ifndef DATACONVERT_H
#define DATACONVERT_H

#include <stdio.h>

#include "bytes.h"
#include "../tests/unit-test.h"

unsigned char hexdigittobyte(char hexdigit);
unsigned char *bytetok(unsigned char *bytes, int len,
		       char *delimiters, int offset,
		       int *tok_len, int *delim_len);
int strtohex(char *hex, char *str);
struct bytes *hextobase64(struct bytes *hex);
char base64todigit(unsigned char value);
char bytetoascii(unsigned char byte);
unsigned char asciitobyte(char ascii);
void bytestostr(char *str, struct bytes *bytes);
void bytestohexstr(char *hexstr, struct bytes *bytes, int uppercase);
int printable(struct bytes *bytes);
void bytetohexs(char *d1, char *d2, unsigned char byte, int uppercase);
char nibbletohexdigit(char nibble, int uppercase);
void base64quadtobytetri(char d1, char d2, char d3, char d4,
		unsigned char *b1, unsigned char *b2, unsigned char *b3);
unsigned char base64digittoval(char digit);
struct bytes *base64filetobytes(FILE *fp);

#endif
