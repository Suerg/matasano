#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <cmocka.h>

/*
 * Only for supression of warning
 */
#include "../src/score.h"
#include "../src/dataconvert.h"

#include "testing-utils.h"

static void test_highest_scoring_xor_rand(void **state)
{
	struct bytes *bytes = bytes_init_from_str("ak;adfkfhn,z");
	struct bytes *answer = bytes_init_from_str("oe5ojhehf`\"t");
	struct bytes *highest = NULL;

	highest = highest_scoring_xor(bytes);

	assert_memory_equal(highest->data, answer->data,
				answer->len * sizeof(answer->data[0]));


	bytes_put(answer);
	bytes_put(bytes);
	bytes_put(highest);
}

static void test_highest_scoring_xor_challenge3(void **state)
{
	struct bytes *bytes = bytes_init_from_hexstr(
			"1b37373331363f78151b7f2b7"
			"83431333d78397828372d363c78373e783a393b3736");
	struct bytes *answer = bytes_init_from_str(
			"Cooking MC's like a pound of bacon");
	struct bytes *highest = NULL;

	highest = highest_scoring_xor(bytes);

	assert_memory_equal(highest->data, answer->data,
				answer->len * sizeof(answer->data[0]));


	bytes_put(answer);
	bytes_put(bytes);
	bytes_put(highest);
}

static void test_highest_scoring_xor_in_file(void **state)
{
	struct bytes *ans = bytes_init_from_str(
			"Now that the party is jumping\n");
	FILE *fp = fopen("../res/4.txt", "r+");
	struct bytes *res = highest_scoring_xor_in_file(fp);

	assert_memory_equal(res->data, ans->data,
			ans->len * sizeof(ans->data[0]));

	bytes_put(ans);
	bytes_put(res);
	fclose(fp);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_highest_scoring_xor_rand),
		cmocka_unit_test(test_highest_scoring_xor_challenge3),
		cmocka_unit_test(test_highest_scoring_xor_in_file)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
