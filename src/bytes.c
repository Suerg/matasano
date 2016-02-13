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
	double len = (double)strlen(hexstr) / 2;
	int len_d = (int)strlen(hexstr) / 2;
	struct bytes *bytes = NULL;
	if (len > (double)len_d)
		len_d++;
	bytes = bytes_create(len_d);
	n1 = n2 = 0;
	i = b = 0;

	for (b = i = 0; i < (int)strlen(hexstr) - 1; b++) {
		if (i == 0 && strlen(hexstr) % 2 != 0) {
			n1 = 0;
			n2 = hexdigittobyte(hexstr[i]);
			bytes->data[b] = bytefromnibbles(n1, n2);
			i++;
		} else {
			n1 = hexdigittobyte(hexstr[i]);
			n2 = hexdigittobyte(hexstr[i + 1]);
			bytes->data[b] = bytefromnibbles(n1, n2);
			i += 2;
		}

	}

	return bytes;
}

struct bytes *bytes_init_from_base64str(char *base64str)
{
	int i, len, b;
	struct bytes *bytes = NULL;
	i = b = 0;
	len = (int)((double)(strlen(base64str) * 3) / 4);
	if (base64str[strlen(base64str) - 2] == '=')
		len -= 2;
	else if (base64str[strlen(base64str) - 1] == '=')
		len--;

	bytes = bytes_create(len);

	for (i = 0; i < (int)strlen(base64str); i += 4) {
		char d1 = base64str[i];
		char d2 = base64str[i + 1];
		char d3 = base64str[i + 2];
		char d4 = base64str[i + 3];
		unsigned char b1, b2, b3;
		b1 = b2 = b3 = 0;
		base64quadtobytetri(d1, d2, d3, d4,
			&b1, &b2, &b3);
		bytes->data[b] = b1;
		if (d3 == '=' && d4 == '=') {
			b++;
		} else if (d4 == '=') {
			bytes->data[b + 1] = b2;
			b += 2;
		} else {
			bytes->data[b + 1] = b2;
			bytes->data[b + 2] = b3;
			b += 3;
		}
	}

	return bytes;
}
