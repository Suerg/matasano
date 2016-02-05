#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/cryptlib.h"

static void test_xorbyte(void **state)
{
	assert_true(xorbyte(0xFF, 'A') == 0xBE);
	assert_true(xorbyte(0x00, 'B') == 'B');
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_xorbyte)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
