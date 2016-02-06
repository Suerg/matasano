#ifndef CRYPTLIB_H
#define CRYPTLIB_H

#include "../tests/unit-test.h"

/*
 * Library functions
 */
double decipherxor(char *deciphered, char *hexstr);
void encipher_repeatingxor(char *enciphered,
		char *str, char *key);
void decipher_repeatingxor(char *deciphered,
		char *enciphered, char *key);

#endif