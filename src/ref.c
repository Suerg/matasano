#include "ref.h"

void ref_init(struct ref *ref)
{
	ref->refcount = 1;
}

void ref_get(struct ref *ref)
{
	ref->refcount++;
}

int ref_sub(struct ref *ref, int count, void (*release)(struct ref *ref))
{
	assert(release != NULL);
	ref->refcount -= count;
	if (ref->refcount < 1) {
		release(ref);
		return 1;
	}
	return 0;
}

int ref_put(struct ref *ref, void (*release)(struct ref *ref))
{
	return ref_sub(ref, 1, release);
}
