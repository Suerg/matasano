#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <cmocka.h>

/*
 * Not actually used!!
 * This is only here to stop compiler yelling at me.
 * Real header/definition comes from .a library.
 */
#include "../src/dataconvert.h"

#include "testing-utils.h"

static void test_hexdigittobyte_letters(void **state)
{
	assert_true(hexdigittobyte('A') == 0xa);
	assert_true(hexdigittobyte('a') == 0xA);
	assert_true(hexdigittobyte('B') == 0xb);
	assert_true(hexdigittobyte('b') == 0xB);
	assert_true(hexdigittobyte('C') == 0xc);
	assert_true(hexdigittobyte('c') == 0xC);
	assert_true(hexdigittobyte('D') == 0xD);
	assert_true(hexdigittobyte('d') == 0xD);
	assert_true(hexdigittobyte('E') == 0xe);
	assert_true(hexdigittobyte('e') == 0xe);
	assert_true(hexdigittobyte('F') == 0xF);
	assert_true(hexdigittobyte('f') == 0xf);
}

static void test_hexdigittobyte_numbers(void **state)
{
	assert_true(hexdigittobyte('0') == 0x0);
	assert_true(hexdigittobyte('1') == 0x1);
	assert_true(hexdigittobyte('2') == 0x2);
	assert_true(hexdigittobyte('3') == 0x3);
	assert_true(hexdigittobyte('4') == 0x4);
	assert_true(hexdigittobyte('5') == 0x5);
	assert_true(hexdigittobyte('6') == 0x6);
	assert_true(hexdigittobyte('7') == 0x7);
	assert_true(hexdigittobyte('8') == 0x8);
	assert_true(hexdigittobyte('9') == 0x9);
}

static void test_hexdigittobyte_assert_failure(void **state)
{
	unsigned char hexval = 0;

	expect_assert_failure(hexval = hexdigittobyte('n'));
	expect_assert_failure(hexval = hexdigittobyte('$'));
	expect_assert_failure(hexval = hexdigittobyte(-1));
	expect_assert_failure(hexval = hexdigittobyte(' '));
	expect_assert_failure(hexval = hexdigittobyte('k'));
	expect_assert_failure(hexval = hexdigittobyte('\0'));

	assert_true(hexval == 0);
}

static void test_bytetok_1(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, offset;
	struct bytes *bytes_1 = bytes_init_from_str("hello world!");
	struct bytes *bytes_2 = bytes_init_from_str(" hello world!");
	struct bytes *bytes_3 = bytes_init_from_str("It was the best of times,"
			" it was the worst of times, it was the age of wisdom,"
			" it was the age of foolishness");
	struct bytes *bytes_4 = bytes_init_from_str("hello world!");
	struct bytes *bytes_5 = bytes_init_from_str("hello world!");
	struct bytes *bytes_6 = bytes_init_from_str("hello world!");
	tok_len = offset = 0;

	pby = bytetok(bytes_1->data, bytes_1->len, delims, &tok_len);
	assert_true(*pby == 'h');
	assert_true(tok_len == 5);

	pby = bytetok(bytes_2->data, bytes_2->len, delims, &tok_len);
	assert_true(*pby == 'h');
	assert_true(tok_len == 5);

	pby = bytetok(bytes_3->data, bytes_3->len, delims, &tok_len);
	offset += tok_len;
	assert_true(*pby == 'I');
	assert_true(tok_len == 2);

	pby = bytetok(pby + offset, bytes_3->len - offset, delims, &tok_len);
	offset += tok_len;
	assert_true(*pby == 'w');
	assert_true(tok_len == 3);

	pby = bytetok(pby + tok_len, bytes_3->len - offset, delims, &tok_len);
	offset += tok_len;
	assert_true(*pby == 't');
	assert_true(tok_len == 3);

	pby = bytetok(pby + tok_len, bytes_3->len - offset, delims, &tok_len);
	offset += tok_len;
	assert_true(*pby == 'b');
	assert_true(tok_len == 4);

	pby = bytetok(pby + tok_len, bytes_3->len - offset, delims, &tok_len);
	offset += tok_len;
	pby = bytetok(pby + tok_len, bytes_3->len - offset, delims, &tok_len);
	offset += tok_len;
	assert_true(*pby == 't');
	assert_true(tok_len == 5);

	bytes_put(bytes_1);
	bytes_put(bytes_2);
	bytes_put(bytes_3);
	bytes_put(bytes_4);
	bytes_put(bytes_5);
	bytes_put(bytes_6);
}
int main(void)
{
	const struct CMUnitTest tests[] = {
		  cmocka_unit_test(test_hexdigittobyte_letters)
		, cmocka_unit_test(test_hexdigittobyte_numbers)
		, cmocka_unit_test(test_hexdigittobyte_assert_failure)
		, cmocka_unit_test(test_bytetok_1)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
