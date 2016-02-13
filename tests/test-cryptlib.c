#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "../src/cryptlib.h"
#include "../src/bytes.h"
#include "../src/datamanip.h"

static void test_decipherxor(void **state)
{
	char *ans = "Cooking MC's like a pound of bacon";
	char *hexstr = "1b37373331363f78151b7f2b783431333d78397"
		       "828372d363c78373e783a393b3736";
	char *deciphered = malloc(strlen(hexstr) * sizeof(deciphered) + 1);
	decipherxor(deciphered, hexstr);

	assert_memory_equal(deciphered, ans, strlen(ans));

	free(deciphered);
}

static void test_decipherxor_file(void **state)
{
	char *ans = "Now that the party is jumping\n";
	char res[600];
	decipherxor_file(res, "../res/4.txt");

	assert_memory_equal(res, ans, strlen(ans));
}

static void test_encipher_repeating_key_xor(void **state)
{
	char *str = "Burning 'em, if you ain't quick and nimble\n"
		    "I go crazy when I hear a cymbal";
	char *ans = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2"
		"a26226324272765272a282b2f20430a652e2c652a3124333a653e2b202763"
		"0c692b20283165286326302e27282f";
	char res[600];
	char *key = "ICE";
	encipher_repeating_key_xor(res, str, key);

	assert_memory_equal(res, ans, strlen(ans));
}

static void test_decipher_repeatingxor(void **state)
{
	char *str = "Burning 'em, if you ain't quick and nimble\n"
		    "I go crazy when I hear a cymbal";
	struct bytes *strbytes = bytes_init_from_str(str);
	struct bytes *keybytes = bytes_init_from_str(
			"ICE");
	struct bytes *enciphered = xor_repeating_key(strbytes, keybytes);
	struct bytes *res = decipher_repeatingxor(enciphered);

	assert_memory_equal(res->data, strbytes->data,
			strbytes->len * sizeof(strbytes->data[0]));

	bytes_put(res);
	bytes_put(enciphered);
	bytes_put(keybytes);
	bytes_put(strbytes);
}

static void test_decipher_repeatingxor_file(void **state)
{
/*
 *        char *ans = "...";
 *        char *res = NULL;
 *        decipher_repeatingxor_file(res, "../res/6.txt");
 *
 *        assert_memory_equal(res, ans, strlen(ans));
 */
}

/*
 *static void test_find_keysize_1(void **state)
 *{
 *        struct bytes *bytes = bytes_init_from_hexstr(
 *                        "9887702584b28e6c71b7bb997e7195bf817a3884bf98353889fa9"
 *                        "f7d34c7bc8a7625c7ae837425cbfa9e7b258eb6cb73308ea8876c"
 *                        "7195bf88703f93b69239718eaecb623094fa9b673e85bb897928c"
 *                        "798997c2586b3853222c7b88e6625c7b18e6525c7a98e762382ae"
 *                        "c535058fb398353894fa89703286af98707188bccb613982fa987"
 *                        "03295bf886c7194af99673e92b48f7c3f80fa8a793dc7ae837071"
 *                        "86b99f7c278eae827022c7b98a67238ebf8f353e89fa83702382f"
 *                        "a8f60238eb48c350688a8877171b0bb99350590b5cb623094fa84"
 *                        "737191b39f743dc7b386653e95ae8a7b3282fa9f7a7188af99353"
 *                        );
 *        int keysize = find_keysize(bytes);
 *
 *        assert_true(keysize == 5);
 *
 *        bytes_put(bytes);
 *
 *}
 */

/*
 *static void test_find_keysize_2(void **state)
 *{
 *        char *key = "ICE";
 *        char *str = "Burning 'em, if you ain't quick and nimble\n"
 *                    "I go crazy when I hear a cymbal";
 *        struct bytes *strbytes = bytes_init_from_str(str);
 *        struct bytes *keybytes = bytes_init_from_str(
 *                        key);
 *        struct bytes *enciphered = xor_repeating_key(strbytes, keybytes);
 *        int keysize = find_keysize(enciphered);
 *
 *        assert_true(keysize == 3);
 *
 *        bytes_put(strbytes);
 *        bytes_put(keybytes);
 *        bytes_put(enciphered);
 *
 *        struct bytes *bytes = bytes_init_from_base64str(
 *                        "HUIfTQsPAh9PE048GmllH0kcDk4TAQsHThsBFkU2AB4BSWQgVB0dQzNTTmVS"
 *                        "BgBHVBwNRU0HBAxTEjwMHghJGgkRTxRMIRpHKwAFHUdZEQQJAGQmB1MANxYG"
 *                        "DBoXQR0BUlQwXwAgEwoFR08SSAhFTmU+Fgk4RQYFCBpGB08fWXh+amI2DB0P"
 *                        "QQ1IBlUaGwAdQnQEHgFJGgkRAlJ6f0kASDoAGhNJGk9FSA8dDVMEOgFSGQEL"
 *                        "QRMGAEwxX1NiFQYHCQdUCxdBFBZJeTM1CxsBBQ9GB08dTnhOSCdSBAcMRVhI"
 *                        "CEEATyBUCHQLHRlJAgAOFlwAUjBpZR9JAgJUAAELB04CEFMBJhAVTQIHAh9P"
 *                        "G054MGk2UgoBCVQGBwlTTgIQUwg7EAYFSQ8PEE87ADpfRyscSWQzT1QCEFMa"
 *                        "TwUWEXQMBk0PAg4DQ1JMPU4ALwtJDQhOFw0VVB1PDhxFXigLTRkBEgcKVVN4"
 *                        );
 *        int *pot_keysizes = NULL;
 *        int len = 0;
 *        find_pot_keysizes(&pot_keysizes, &len, bytes);
 *
 *        free_cryptlib(pot_keysizes);
 *        bytes_put(bytes);
 *}
 */

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_decipherxor),
		cmocka_unit_test(test_decipherxor_file),
		cmocka_unit_test(test_encipher_repeating_key_xor),
		cmocka_unit_test(test_decipher_repeatingxor_file),
		cmocka_unit_test(test_decipher_repeatingxor)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
