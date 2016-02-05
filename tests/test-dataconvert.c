#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>

#define UNIT_TESTING
#include "unit-test.h"

#include "../src/dataconvert.h"


static void test_strtobytes(void **state)
{
	char str_1[] = "hello world!";
	unsigned char bytes_1_ans[] =  {'h', 'e', 'l', 'l', 'o', ' ',
				'w', 'o', 'r', 'l', 'd', '!'};
	unsigned char *bytes_1_res = malloc(strlen(str_1)
					* sizeof(bytes_1_res[0]));
	strtobytes(bytes_1_res, str_1);

	assert_memory_equal(bytes_1_res, bytes_1_ans, strlen(str_1) *
				sizeof(bytes_1_res[0]));

	free(bytes_1_res);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_strtobytes)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
