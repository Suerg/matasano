#ifndef CRYPTLIB_H
#define CRYPTLIB_H

#include "../tests/unit-test.h"

/*
 * Library functions
 */
void decipherxor(char *deciphered, char *hexstr);
void decipherxor_file(char *deciphered, char *fname);

void encipher_repeating_key_xor(char *enciphered, char *str, char *key);
void decipher_repeatingxor(char *deciphered,
		char *enciphered, char *key);

#endif
