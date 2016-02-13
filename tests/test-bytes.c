#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>

#include "../src/bytes.h"

static void test_bytes_create(void **state)
{
	struct bytes *b1 = bytes_create(0);
	struct bytes *b2 = bytes_create(1);
	struct bytes *b3 = bytes_create(500);

	assert_true(b1->len == 0);
	assert_true(b2->len == 1);
	assert_true(b3->len == 500);

	bytes_put(b1);
	bytes_put(b2);
	bytes_put(b3);
}

static void test_bytes_put(void **state)
{
	struct bytes *bytes = bytes_create(1);
	unsigned char *cpy = NULL;
	cpy = bytes->data;

	bytes = bytes_get(bytes);
	bytes_put(bytes);
	assert_true(bytes->ref.refcount == 1);

	assert_true(cpy == bytes->data);
	bytes_put(bytes);
	assert_true(cpy != bytes->data);
}

static void test_bytes_get(void **state)
{
	struct bytes *bytes = bytes_create(1);

	assert_true(bytes->ref.refcount == 1);

	bytes = bytes_get(bytes);

	assert_true(bytes->ref.refcount == 2);

	bytes = bytes_get(bytes);

	assert_true(bytes->ref.refcount == 3);

	bytes_put(bytes);
	bytes_put(bytes);
	bytes_put(bytes);
}

static void test_bytes_init_from_str(void **state)
{
	char *str = "hello world!";
	struct bytes *bytes = bytes_init_from_str(str);

	assert_memory_equal(bytes->data, str, strlen(str));

	bytes_put(bytes);
}

static void test_bytes_init_from_hexstr(void **state)
{
	char *hexstr = "3b2a1c5d";
	unsigned char hexdata[] = {
		0x3b, 0x2a, 0x1c, 0x5d
	};
	struct bytes *bytes = bytes_init_from_hexstr(hexstr);

	assert_memory_equal(bytes->data, hexdata, bytes->len *
			sizeof(bytes->data[0]));

	bytes_put(bytes);
}

static void test_bytes_init_from_hexstr_zeropad(void **state)
{
	char *hexstr = "b2a1c5d";
	unsigned char hexdata[] = {
		0xb, 0x2a, 0x1c, 0x5d
	};
	struct bytes *bytes = bytes_init_from_hexstr(hexstr);

	assert_memory_equal(bytes->data, hexdata, bytes->len *
			sizeof(bytes->data[0]));

	bytes_put(bytes);
}

static void test_bytes_init_from_base64str(void **state)
{
	char *base64str = "Jk8DCkkcC3hFMQIEC0EbAVIqCFZBO1IdBgZUVA"
		"4QTgUWSR4QJwwRTWM=";
	unsigned char data[] = {
	 0x26, 0x4f, 0x03, 0x0a, 0x49, 0x1c, 0x0b, 0x78, 0x45, 0x31, 0x02,
	 0x04, 0x0b, 0x41, 0x1b, 0x01, 0x52, 0x2a, 0x08, 0x56, 0x41, 0x3b,
	 0x52, 0x1d, 0x06, 0x06, 0x54, 0x54, 0x0e, 0x10, 0x4e, 0x05, 0x16,
	 0x49, 0x1e, 0x10, 0x27, 0x0c, 0x11, 0x4d, 0x63
	};

	struct bytes *bytes = bytes_init_from_base64str(base64str);

	assert_memory_equal(bytes->data, data, bytes->len *
			sizeof(bytes->data[0]));

	bytes_put(bytes);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_bytes_create),
		cmocka_unit_test(test_bytes_put),
		cmocka_unit_test(test_bytes_get),
		cmocka_unit_test(test_bytes_init_from_str),
		cmocka_unit_test(test_bytes_init_from_hexstr),
		cmocka_unit_test(test_bytes_init_from_hexstr_zeropad),
		cmocka_unit_test(test_bytes_init_from_base64str)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
