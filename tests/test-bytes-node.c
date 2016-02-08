#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <cmocka.h>

#include "../src/bytes-node.h"

#include "testing-utils.h"

static void test_bytes_node_create(void **state)
{
	struct bytes *bytes = bytes_init_from_str("hello world!");
	struct bytes_node *head = bytes_node_create(bytes);

	assert_true(head->next == NULL);
	assert_memory_equal(head->bytes->data, bytes->data, bytes->len *
						sizeof(bytes->data[0]));
	assert_true(head->bytes->len == bytes->len);

	bytes_put(bytes);
	bytes_node_put(head);
}

static void test_bytes_node_put(void **state)
{
	struct bytes_node *head = bytes_node_init_from_str(
			"hello world!");
	struct bytes *head_bytes = NULL;
	head_bytes = head->bytes;

	assert_true(head_bytes == head->bytes);

	bytes_node_put(head);

	assert_true(head_bytes != head->bytes);


}

static void test_bytes_node_get(void **state)
{
	struct bytes_node *head = bytes_node_init_from_str(
			"the quick brown fox...");
	head = bytes_node_get(head);

	assert_true(head->ref.refcount == 2);

	head = bytes_node_get(head);

	assert_true(head->ref.refcount == 3);

	bytes_node_put(head);
	bytes_node_put(head);
	bytes_node_put(head);
}

static void test_bytes_node_add(void **state)
{
	struct bytes_node *head = bytes_node_init_from_str("hello");
	struct bytes *bytes_1 = bytes_init_from_str("world");
	struct bytes *bytes_2  = bytes_init_from_str("now we're talking");
	bytes_node_add(head, bytes_1);

	assert_true(head->next != NULL);
	assert_memory_equal(head->next->bytes->data, bytes_1->data,
			bytes_1->len * sizeof(bytes_1->data[0]));
	assert_true(head->next->next == NULL);

	bytes_node_add(head, bytes_2);

	assert_true(head->next->next != NULL);
	assert_memory_equal(head->next->next->bytes->data, bytes_2->data,
			bytes_2->len * sizeof(bytes_2->data[0]));
	assert_true(head->next->next->next == NULL);

	bytes_put(bytes_1);
	bytes_put(bytes_2);
	bytes_node_put(head);
}

static void test_bytes_node_init_from_str(void **state)
{
	struct bytes *bytes = bytes_init_from_str("all for one");
	struct bytes_node *node = bytes_node_init_from_str("all for one");

	assert_memory_equal(bytes->data, node->bytes->data, bytes->len
					* sizeof(bytes->data[0]));

	bytes_put(bytes);
	bytes_node_put(node);
}

static void test_bytes_node_element_at(void **state)
{
	struct bytes_node *head = bytes_node_init_from_str("It");
	struct bytes *word_2 = bytes_init_from_str("was");
	struct bytes *word_3 = bytes_init_from_str("the");
	struct bytes *word_4 = bytes_init_from_str("best");

	bytes_node_add(head, word_2);
	bytes_node_add(head, word_3);
	bytes_node_add(head, word_4);

	assert_memory_equal(bytes_node_element_at(head, 1)->bytes->data,
			    word_2->data, word_2->len *
			    sizeof(word_2->data[0]));
	assert_memory_equal(bytes_node_element_at(head, 2)->bytes->data,
			    word_3->data, word_3->len *
			    sizeof(word_3->data[0]));
	assert_memory_equal(bytes_node_element_at(head, 3)->bytes->data,
			    word_4->data, word_4->len *
			    sizeof(word_4->data[0]));

	bytes_put(word_4);
	bytes_put(word_3);
	bytes_put(word_2);
	bytes_node_put(head);
}

static void test_bytes_node_init_as_tokens_1(void **state)
{
	struct bytes *tok_1 = bytes_init_from_str("*/");
	struct bytes *tok_2 = bytes_init_from_str("'p");
	struct bytes *tok_3 = bytes_init_from_str("!*");
	struct bytes *bytes = bytes_init_from_str("*/-'p !*- ");
	struct bytes_node *head = bytes_node_init_as_tokens(bytes, " ,.-:;");

	assert_memory_equal(head->bytes->data, tok_1->data,
				tok_1->len * sizeof(tok_1->data[0]));
	assert_memory_equal(head->next->bytes->data, tok_2->data,
				tok_2->len * sizeof(tok_2->data[0]));
	assert_memory_equal(head->next->next->bytes->data, tok_3->data,
				tok_3->len * sizeof(tok_3->data[0]));
	bytes_put(tok_1);
	bytes_put(tok_2);
	bytes_put(tok_3);
	bytes_put(bytes);
	bytes_node_put(head);
}
static void test_bytes_node_init_as_tokens(void **state)
{
	struct bytes *tok_1 = bytes_init_from_str("It");
	struct bytes *tok_2 = bytes_init_from_str("was");
	struct bytes *tok_3 = bytes_init_from_str("the");
	struct bytes *tok_4 = bytes_init_from_str("best");
	struct bytes *tok_5 = bytes_init_from_str("of");
	struct bytes *tok_6 = bytes_init_from_str("times");

	struct bytes *bytes = bytes_init_from_str("It was the best of times.");
	struct bytes_node *head = bytes_node_init_as_tokens(bytes, " .");

	assert_memory_equal(head->bytes->data, tok_1->data,
				tok_1->len * sizeof(tok_1->data[0]));
	assert_memory_equal(head->next->bytes->data, tok_2->data,
				tok_2->len * sizeof(tok_2->data[0]));
	assert_memory_equal(head->next->next->bytes->data, tok_3->data,
				tok_3->len * sizeof(tok_3->data[0]));
	assert_memory_equal(head->next->next->next->bytes->data, tok_4->data,
				tok_4->len * sizeof(tok_4->data[0]));
	assert_memory_equal(head->next->next->next->next->bytes->data,
			tok_5->data, tok_5->len * sizeof(tok_5->data[0]));
	assert_memory_equal(head->next->next->next->next->next->bytes->data,
			tok_6->data, tok_6->len * sizeof(tok_6->data[0]));

	bytes_put(tok_1);
	bytes_put(tok_2);
	bytes_put(tok_3);
	bytes_put(tok_4);
	bytes_put(tok_5);
	bytes_put(tok_6);
	bytes_put(bytes);
	bytes_node_put(head);
}

static void test_bytes_node_len(void **state)
{
	struct bytes *bytes = bytes_init_from_str("It was the best of times.");
	struct bytes_node *head = bytes_node_init_as_tokens(bytes, " .");

	assert_true(bytes_node_len(head) == 6);

	bytes_put(bytes);
	bytes_node_put(head);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_bytes_node_create)
		, cmocka_unit_test(test_bytes_node_put)
		, cmocka_unit_test(test_bytes_node_get)
		, cmocka_unit_test(test_bytes_node_add)
		, cmocka_unit_test(test_bytes_node_init_from_str)
		, cmocka_unit_test(test_bytes_node_element_at)
		, cmocka_unit_test(test_bytes_node_init_as_tokens)
		, cmocka_unit_test(test_bytes_node_init_as_tokens_1)
		, cmocka_unit_test(test_bytes_node_len)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
