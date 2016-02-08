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

static void test_highest_scoring_xor(void **state)
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

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_highest_scoring_xor)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
