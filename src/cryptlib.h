#ifndef CRYPTLIB_H
#define CRYPTLIB_H

#define KEYSIZE 40

#include "../tests/unit-test.h"
#include "../src/bytes.h"

/*
 * Library functions
 */
void decipherxor(char *deciphered, char *hexstr);
void decipherxor_file(char *deciphered, char *fname);

void encipher_repeating_key_xor(char *enciphered, char *str, char *key);
void decipher_repeatingxor_file(char *deciphered, char *fname);
struct bytes *decipher_repeatingxor(struct bytes *enciphered);

#endif
