#include "testing-utils.h"

void *init_item_from(void *from, size_t num_bytes)
{
	void *item = NULL;
	item = malloc(num_bytes);
	assert(item != NULL);
	memcpy(item, from, num_bytes);

	return item;
}

void *init_item(size_t num_bytes)
{
	void *item = NULL;
	item = malloc(num_bytes);
	assert(item != NULL);

	return item;
}
