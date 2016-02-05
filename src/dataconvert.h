#ifndef DATACONVERT_H
#define DATACONVERT_H

#define START_CIPHER (0)
#define END_CIPHER (127)
#define CIPHERS ((END_CIPHER) - (START_CIPHER))
unsigned char hexdigittobyte(char hexdigit);
unsigned char * bytetok(unsigned char *bytes, char *delimiters,
		int len, int *tok_len);
void hexstrtonibbles(unsigned char *nibbles, char *hexstr);
int strtohex(char *hex, char *str);
int strtobytes(unsigned char *bytes, char *str);
int hexstrtobytes(unsigned char *bytes, char *hexstr);
int hextobase64(char *base64, char *hex, int bytes);
int nibblestobytes(unsigned char *bytes, unsigned char *nibbles, int len);
unsigned char nibblestobyte(unsigned char n1, unsigned char n2);
void base64tostr(char *str, char *base64, int digits);
char base64todigit(unsigned char value);
char bytetoascii(unsigned char byte);
unsigned char asciitobyte(char ascii);

#endif
