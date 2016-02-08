#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bytes-node.h"
#include "dataconvert.h"

static void bytes_node_free(struct ref *ref)
{
	struct bytes_node *node = container_of(ref, struct bytes_node, ref);
	bytes_put(node->bytes);
	free(node);
	node = NULL;
}

void bytes_node_put(struct bytes_node *node)
{
	if (node != NULL) {
		bytes_node_put(node->next);
		ref_put(&node->ref, bytes_node_free);
	}
}

struct bytes_node *bytes_node_get(struct bytes_node *node)
{
	ref_get(&node->ref);
	return node;
}

struct bytes_node *bytes_node_create(struct bytes *bytes)
{
	struct bytes_node *node = malloc(sizeof(*node));
	ref_init(&node->ref);

	node->next = NULL;
	node->bytes = bytes_create(bytes->len);
	memcpy(node->bytes->data, bytes->data, bytes->len *
					       sizeof(bytes->data[0]));

	return node;
}

void bytes_node_add(struct bytes_node *node, struct bytes *bytes)
{
	if (node->next == NULL)
		node->next = bytes_node_create(bytes);
	else
		bytes_node_add(node->next, bytes);
}

struct bytes_node *bytes_node_init_from_str(char *str)
{
	struct bytes *bytes = bytes_init_from_str(str);
	struct bytes_node *node = bytes_node_create(bytes);
	bytes_put(bytes);
	return node;
}

struct bytes_node *bytes_node_init_as_tokens(struct bytes *bytes, char *delims)
{
	struct bytes_node *head = NULL;
	unsigned char *pby = NULL;
	struct bytes *token = NULL;
	int tok_len, offset, delim_len;
	tok_len = offset = delim_len = 0;

	pby = bytetok(bytes->data, bytes->len, delims,
			offset, &tok_len, &delim_len);
	token = bytes_create(tok_len);
	memcpy(token->data, pby, tok_len * sizeof(bytes->data[0]));
	head = bytes_node_create(token);
	bytes_put(token);

	while (pby) {
		offset += (tok_len + delim_len);
		pby = bytetok(bytes->data, bytes->len, delims,
				offset, &tok_len, &delim_len);
		if (!pby)
			break;
		token = bytes_create(tok_len);
		memcpy(token->data, pby, tok_len * sizeof(bytes->data[0]));

		bytes_node_add(head, token);

		bytes_put(token);
	}

	return head;
}

struct bytes_node *bytes_node_element_at(struct bytes_node *node, int index)
{
	if (index == 0)
		return node;
	else
		return bytes_node_element_at(node->next, index - 1);
}

int bytes_node_len(struct bytes_node *node)
{
	if (node == NULL)
		return 0;
	else
		return bytes_node_len(node->next) + 1;
}
