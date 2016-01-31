#ifndef CRYPTLIB_H
#define CRYPTLIB_H

#define CIPHERS (127)
#define SEPARATORS (" ,.-:;")

void xortwo(char *xored, char *buf1, char *buf2, int num);
void strtohex_nibbles(char *nibbles, char *str);
int strtohex_bytes(char *bytes, char *str);
int hextobase64(char *base64, char *hex, int bytes);
int nibblestobytes(char *bytes, char *nibbles, int numb);
char nibblestobyte(char n1, char n2);
void base64tostr(char *str, char *base64, int digits);
char base64todigit(char value);
char xorbyte(char byte, char xorwith);
void xorbytes(char *xoerd, char *bytes, int num, char xorwith);
void scorebytes(double *scores, char *bytes, int num);
double scorechars(char *chars);
double scorewords(char **chars, int num);
double score_letterfreq(char *chars);
double score_start(char *word);
double score_end(char *word);
int countc(char *chars, char c);
double decipherxor(char *deciphered, char *hexstr);

#endif
