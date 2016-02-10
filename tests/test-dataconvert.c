#include <stdio.h>
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

static int setup_bytetok_1(void **state)
{
	struct bytes *bytes = bytes_init_from_str("hello world!");
	*state = bytes;
	return 0;
}

static int setup_bytetok_2(void **state)
{
	struct bytes *bytes = bytes_init_from_str(" hello world!");
	*state = bytes;
	return 0;
}

static int setup_bytetok_3(void **state)
{
	struct bytes *bytes = bytes_init_from_str("It was the best of times,");
	*state = bytes;
	return 0;
}

static int setup_bytetok_4(void **state)
{
	struct bytes *bytes = bytes_init_from_str("adfl;kjafk");
	*state = bytes;
	return 0;
}

static int setup_bytetok_5(void **state)
{
	struct bytes *bytes = bytes_init_from_str("");
	*state = bytes;
	return 0;
}

static int setup_bytetok_6(void **state)
{
	struct bytes *bytes = bytes_init_from_str("hello");
	*state = bytes;
	return 0;
}

static int teardown_bytetok(void **state)
{
	struct bytes *bytes = *state;
	bytes_put(bytes);
	return 0;
}

static void test_bytetok_1(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, offset, delim_len;
	struct bytes *bytes = *state;
	tok_len = offset = delim_len = 0;

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	assert_true(*pby == 'h');
	fprintf(stderr,
		"got tok len: %d, delim_len: %d\n",
		tok_len, delim_len);
	assert_true(tok_len == 5);
	assert_true(delim_len == 1);

}

static void test_bytetok_2(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, delim_len, offset;
	struct bytes *bytes = *state;
	tok_len = delim_len = offset = 0;

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 'I');
	assert_true(tok_len == 2);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 'w');
	assert_true(tok_len == 3);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 't');
	assert_true(tok_len == 3);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 'b');
	assert_true(tok_len == 4);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 't');
	assert_true(tok_len == 5);
}

static void test_bytetok_3(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, delim_len, offset;
	struct bytes *bytes = *state;
	tok_len = delim_len = offset = 0;

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 'a');
	assert_true(tok_len == 4);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	offset += (tok_len + delim_len);
	assert_true(*pby == 'k');
	assert_true(tok_len == 5);

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	assert_true(pby == NULL);
}

static void test_bytetok_4(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, delim_len, offset;
	struct bytes *bytes = *state;
	tok_len = delim_len = offset = 0;

	pby = bytetok(pby, bytes->len, delims,
			offset, &tok_len, &delim_len);
	assert_true(pby == NULL);
}

static void test_bytetok_5(void **state)
{
	char *delims = " ,.;:-!";
	unsigned char *pby = NULL;
	int tok_len, delim_len, offset;
	struct bytes *bytes = *state;
	tok_len = delim_len = offset = 0;

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	assert_true(*pby == 'h');
	assert_true(tok_len == 5);

	offset = (tok_len + delim_len);
	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	assert_true(pby == NULL);
}
static void test_hextobase64_1(void **state)
{
	struct bytes *bytes = bytes_init_from_hexstr("4d616e");
	struct bytes *base64 = hextobase64(bytes);

	assert_true(base64->data[0] == 19);
	assert_true(base64->data[1] == 22);
	assert_true(base64->data[2] == 5);
	assert_true(base64->data[3] == 46);

	bytes_put(bytes);
	bytes_put(base64);
}

static void test_hextobase64_2(void **state)
{
	struct bytes *bytes = bytes_init_from_hexstr(
		"49276d206b696c6c696e6720796f757220627261696e206c69"
		"6b65206120706f69736f6e6f7573206d757368726f6f6d");
	struct bytes *base64 = hextobase64(bytes);
	unsigned char answer[] = {
		18, 18, 29, 45, 8, 6, 45, 41, 27, 6, 49, 41, 27, 38,
		28, 32, 30, 22, 61, 53, 28, 34, 1, 34, 28, 38, 5, 41,
		27, 34, 1, 44, 26, 22, 45, 37, 8, 6, 4, 32, 28, 6, 61,
		41, 28, 54, 61, 46, 27, 55, 21, 51, 8, 6, 53, 53, 28, 54,
		33, 50, 27, 54, 61, 45
	};

	printf("55: %d", base64->data[55]);
	assert_memory_equal(answer, base64->data, base64->len *
				    sizeof(base64->data[0]));
	bytes_put(bytes);
	bytes_put(base64);
}

static void test_base64todigit(void **state)
{
	unsigned char v1 = 0;
	unsigned char v2 = 8;
	unsigned char v3 = 15;
	unsigned char v4 = 25;
	unsigned char v5 = 30;
	unsigned char v6 = 38;
	unsigned char v7 = 50;
	unsigned char v8 = 54;
	unsigned char v9 = 60;
	unsigned v10 = 62;
	unsigned char v11 = 63;

	assert_true(base64todigit(v1) == 'A');
	assert_true(base64todigit(v2) == 'I');
	assert_true(base64todigit(v3) == 'P');
	assert_true(base64todigit(v4) == 'Z');
	assert_true(base64todigit(v5) == 'e');
	assert_true(base64todigit(v6) == 'm');
	assert_true(base64todigit(v7) == 'y');
	assert_true(base64todigit(v8) == '2');
	assert_true(base64todigit(v9) == '8');
	assert_true(base64todigit(v10) == '+');
	assert_true(base64todigit(v11) == '/');
}

static void test_bytetoascii_success(void **state)
{
	assert_true(bytetoascii(0x33) == '3');
	assert_true(bytetoascii(0x25) == '%');
	assert_true(bytetoascii(0x11) == '\x11');
	assert_true(bytetoascii(0x0d) == '\r');
	assert_true(bytetoascii(7) == '\a');
	assert_true(bytetoascii(8) == '\b');
	assert_true(bytetoascii(10) == '\n');
	assert_true(bytetoascii(0) == '\0');
	assert_true(bytetoascii(0x2b) == '+');
	assert_true(bytetoascii(0x3f) == '?');
	assert_true(bytetoascii(0x6f) == 'o');
	assert_true(bytetoascii(0x7d) == '}');
	assert_true(bytetoascii(0x65) == 'e');
	assert_true(bytetoascii(0x52) == 'R');
	assert_true(bytetoascii(0x55) == 'U');
}

static void test_bytetoascii_assert_failure(void **state)
{
	char save = 0;
	expect_assert_failure(save = bytetoascii(128));
	expect_assert_failure(save = bytetoascii(190));
	expect_assert_failure(save = bytetoascii(200));
	expect_assert_failure(save = bytetoascii(234));
	expect_assert_failure(save = bytetoascii(255));
	assert_true(save == 0);
}

static void test_asciitobyte_success(void **state)
{
	assert_true(asciitobyte('3') == 0x33);
	assert_true(asciitobyte('\x11') == 0x11);
	assert_true(asciitobyte('\r') == 0x0d);
	assert_true(asciitobyte('\a') == 7);
	assert_true(asciitobyte('\b') == 8);
	assert_true(asciitobyte('\n') == 10);
	assert_true(asciitobyte('\0') == 0);
	assert_true(asciitobyte('+') == 0x2b);
	assert_true(asciitobyte('?') == 0x3f);
	assert_true(asciitobyte('o') == 0x6f);
	assert_true(asciitobyte('}') == 0x7d);
	assert_true(asciitobyte('e') == 0x65);
	assert_true(asciitobyte('R') == 0x52);
	assert_true(asciitobyte('U') == 0x55);
}

static void test_asciitobyte_assert_failure(void **state)
{
	unsigned char save = 0;
	expect_assert_failure(save = asciitobyte(-20));
	expect_assert_failure(save = asciitobyte(-39));
	expect_assert_failure(save = asciitobyte(-54));
	expect_assert_failure(save = asciitobyte(-100));
	expect_assert_failure(save = asciitobyte(-127));
	assert_true(save == 0);
}

static void test_bytestostr(void **state)
{
	char *ans_1 = "this is how the world ends...";
	char *ans_2 = "the quick brown fox jumps over the lazy dog";
	char *ans_3 = "that's all folks";

	struct bytes *b_1 = bytes_init_from_str(ans_1);
	struct bytes *b_2 = bytes_init_from_str(ans_2);
	struct bytes *b_3 = bytes_init_from_str(ans_3);

	char *res_1 = malloc(b_1->len * sizeof(*res_1) + 1);
	char *res_2 = malloc(b_2->len * sizeof(*res_2) + 1);
	char *res_3 = malloc(b_3->len * sizeof(*res_3) + 1);

	bytestostr(res_1, b_1);
	bytestostr(res_2, b_2);
	bytestostr(res_3, b_3);

	assert_memory_equal(res_1, ans_1, strlen(ans_1));
	assert_memory_equal(res_2, ans_2, strlen(ans_2));
	assert_memory_equal(res_3, ans_3, strlen(ans_3));

	free(res_1);
	free(res_2);
	free(res_3);
	bytes_put(b_1);
	bytes_put(b_2);
	bytes_put(b_3);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		  cmocka_unit_test(test_hexdigittobyte_letters)
		, cmocka_unit_test(test_hexdigittobyte_numbers)
		, cmocka_unit_test(test_hexdigittobyte_assert_failure)
		, cmocka_unit_test_setup_teardown(test_bytetok_1,
				setup_bytetok_1, teardown_bytetok)
		, cmocka_unit_test_setup_teardown(test_bytetok_1,
				setup_bytetok_2, teardown_bytetok)
		, cmocka_unit_test_setup_teardown(test_bytetok_2,
				setup_bytetok_3, teardown_bytetok)
		, cmocka_unit_test_setup_teardown(test_bytetok_3,
				setup_bytetok_4, teardown_bytetok)
		, cmocka_unit_test_setup_teardown(test_bytetok_4,
				setup_bytetok_5, teardown_bytetok)
		, cmocka_unit_test_setup_teardown(test_bytetok_5,
				setup_bytetok_6, teardown_bytetok)
		, cmocka_unit_test(test_hextobase64_1)
		, cmocka_unit_test(test_hextobase64_2)
		, cmocka_unit_test(test_base64todigit)
		, cmocka_unit_test(test_bytetoascii_success)
		, cmocka_unit_test(test_bytetoascii_assert_failure)
		, cmocka_unit_test(test_asciitobyte_success)
		, cmocka_unit_test(test_asciitobyte_assert_failure)
		, cmocka_unit_test(test_bytestostr)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
