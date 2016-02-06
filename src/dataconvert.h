#ifndef DATACONVERT_H
#define DATACONVERT_H

#define START_CIPHER (0)
#define END_CIPHER (127)
#define CIPHERS ((END_CIPHER) - (START_CIPHER))

#include "bytes.h"
#include "../tests/unit-test.h"

unsigned char hexdigittobyte(char hexdigit);
unsigned char *bytetok(unsigned char *bytes, int len,
		       char *delimiters, int *tok_len);
int strtohex(char *hex, char *str);
struct bytes *hextobase64(struct bytes *hex);
void base64tostr(char *str, struct bytes *base64);
char base64todigit(unsigned char value);
char bytetoascii(unsigned char byte);
unsigned char asciitobyte(char ascii);

#endif
