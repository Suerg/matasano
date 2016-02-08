#ifndef BYTE_LIST_H
#define BYTE_LIST_H

#include "ref.h"
#include "bytes.h"

struct bytes_node {
	struct bytes *bytes;
	struct ref ref;
	struct bytes_node *next;
};

void bytes_node_put(struct bytes_node *node);
struct bytes_node *bytes_node_get(struct bytes_node *node);
struct bytes_node *bytes_node_create(struct bytes *bytes);
void bytes_node_add(struct bytes_node *node, struct bytes *bytes);
struct bytes_node *bytes_node_init_from_str(char *str);
struct bytes_node *bytes_node_init_as_tokens(struct bytes *bytes,
		char *delims);
struct bytes_node *bytes_node_element_at(struct bytes_node *node, int index);
int bytes_node_len(struct bytes_node *node);

#endif
