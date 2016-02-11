#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "../src/cryptlib.h"

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

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_decipherxor),
		cmocka_unit_test(test_decipherxor_file),
		cmocka_unit_test(test_encipher_repeating_key_xor)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
