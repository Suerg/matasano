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

static void test_xor_repeating_key(void **state)
{
	struct bytes *bytes = bytes_init_from_str(
			"Burning 'em, if you ain't quick and nimble\n"
			"I go crazy when I hear a cymbal"
			);
	struct bytes *ans = bytes_init_from_hexstr(
			"0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d633"
			"43c2a26226324272765272a282b2f20430a652e2c652a3124333a"
			"653e2b2027630c692b20283165286326302e27282f"
			);
	struct bytes *key = bytes_init_from_str("ICE");
	struct bytes *enciphered = xor_repeating_key(bytes, key);

	assert_memory_equal(enciphered->data, ans->data, ans->len *
			sizeof(ans->data[0]));

	bytes_put(bytes);
	bytes_put(ans);
	bytes_put(key);
	bytes_put(enciphered);
}

static void test_edit_distance(void **state)
{
	struct bytes *b1 = bytes_init_from_str(
			"this is a test\n");
	struct bytes *b2 = bytes_init_from_str(
			"wokka wokka!!!\n");
	int dist = 0;

	dist = edit_distance(b1, b2);

	assert_true(dist == 37);

	bytes_put(b1);
	bytes_put(b2);
}

static void test_edit_distance_count(void **state)
{
	struct bytes *bytes = bytes_init_from_str(
			"this is a test\nthis is a test\n");
	struct bytes *b1 = bytes_init_from_str(
			"this is a test\nwokka wokka!!!\n");
	int dist, dist1;
	dist = dist1 = 0;

	dist = edit_distance_count(bytes, 15);
	assert_true(dist == 0);

	dist1 = edit_distance_count(b1, 15);
	assert_true(dist1 == 37);

	bytes_put(bytes);
	bytes_put(b1);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_xorbyte),
		cmocka_unit_test(test_xortwo),
		cmocka_unit_test(test_xorbytes),
		cmocka_unit_test(test_xor_repeating_key),
		cmocka_unit_test(test_edit_distance),
		cmocka_unit_test(test_edit_distance_count)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
