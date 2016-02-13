#ifndef BYTES_H
#define BYTES_H

#include "ref.h"
#include "../tests/unit-test.h"

struct bytes {
	struct ref ref;
	unsigned char *data;
	int len;
};

void bytes_put(struct bytes *bytes);
struct bytes *bytes_get(struct bytes *bytes);
struct bytes *bytes_create(int len);
struct bytes *bytes_init_from_str(char *str);
struct bytes *bytes_init_from_hexstr(char *hexstr);
struct bytes *bytes_init_from_base64str(char *base64str);

#endif
