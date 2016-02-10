#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <cmocka.h>

#include "../src/datamanip.h"
#include "../src/bytes.h"

static void test_xortwo(void **state)
{
	struct bytes *answer = bytes_init_from_hexstr(
			"746865206b696420646f6e277420706c6179");
	struct bytes *b1 = bytes_init_from_hexstr(
			"1c0111001f010100061a024b53535009181c");
	struct bytes *b2 = bytes_init_from_hexstr(
			"686974207468652062756c6c277320657965");
	struct bytes *xored = bytes_create(b1->len);

	xortwo(xored, b1, b2);

	assert_memory_equal(xored->data, answer->data,
			answer->len * sizeof(answer->data[0]));

	bytes_put(answer);
	bytes_put(b1);
	bytes_put(b2);
	bytes_put(xored);

}

static void test_xorbyte(void **state)
{
	unsigned char answer = 0x77;
	unsigned char b1 = 0xD6;
	unsigned char b2 = 0xA1;
	unsigned char result = 0;

	result = xorbyte(b1, b2);

	assert_true(result == answer);
}

static void test_xorbytes(void **state)
{
	struct bytes *bytes = bytes_init_from_hexstr(
			"1b37373331363f78151b7f2b783431333d783978283"
			"72d363c78373e783a393b3736");
	struct bytes *answer = bytes_init_from_str(
			"Cooking MC's like a pound of bacon"
			);
	struct bytes *xored = bytes_create(bytes->len);

	xorbytes(xored, bytes, 'X');

	assert_memory_equal(xored->data, answer->data,
			answer->len * sizeof(answer->data[0]));

	bytes_put(bytes);
	bytes_put(answer);
	bytes_put(xored);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_xorbyte)
		, cmocka_unit_test(test_xortwo)
		, cmocka_unit_test(test_xorbytes)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
