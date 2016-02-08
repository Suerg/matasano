#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bytes.h"
#include "dataconvert.h"

static void bytes_free(struct ref *ref)
{
	struct bytes *bytes = container_of(ref, struct bytes, ref);
	free(bytes->data);
	bytes->data = NULL;
	free(bytes);
	bytes = NULL;
}

void bytes_put(struct bytes *bytes)
{
	ref_put(&bytes->ref, bytes_free);
}

struct bytes *bytes_get(struct bytes *bytes)
{
	ref_get(&bytes->ref);
	return bytes;
}

struct bytes *bytes_create(int len)
{
	struct bytes *bytes = malloc(sizeof(*bytes));

	bytes->len = len;
	ref_init(&bytes->ref);
	bytes->data = malloc(len * sizeof(*(bytes->data)));

	return bytes;
}

struct bytes *bytes_init_from_str(char *str)
{
	int i = 0;
	struct bytes *bytes = bytes_create((int)strlen(str));

	for (i = 0; i < (int)strlen(str); i++)
		bytes->data[i] = asciitobyte(str[i]);

	return bytes;
}

static unsigned char bytefromnibbles(unsigned char n1, unsigned n2)
{
	return n1 << 4 | n2;
}

struct bytes *bytes_init_from_hexstr(char *hexstr)
{
	int i, b;
	unsigned char n1, n2;
	struct bytes *bytes = bytes_create((int)strlen(hexstr) / 2);
	n1 = n2 = 0;
	i = b = 0;

	for (b = i = 0; i < (int)strlen(hexstr) - 1; b++, i += 2) {
		if (i == 0 && strlen(hexstr) % 2 != 0) {
			n1 = 0;
			n2 = hexdigittobyte(hexstr[i]);
			bytes->data[b] = bytefromnibbles(n1, n2);
		} else {
			n1 = hexdigittobyte(hexstr[i]);
			n2 = hexdigittobyte(hexstr[i + 1]);
			bytes->data[b] = bytefromnibbles(n1, n2);
		}
	}

	return bytes;
}
