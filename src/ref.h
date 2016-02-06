#ifndef REF_H
#define REF_H

/*
 * ref.h - reference counting module
 * based on kref.h which can be found at
 * https://github.com/torvalds/linux/blob/master/include/linux/kref.h
 */

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))

#include <assert.h>
#include <stddef.h>

struct ref {
	int refcount;
};

/*
 * ref_init - initialize object.
 * @ref: object.
 */
void ref_init(struct ref *ref);

/*
 * ref_get - increment refcount for object.
 * @ref: object.
 */
void ref_get(struct ref *ref);

/*
 * ref_sub - subtract a number of refcounts for object.
 * @ref: object.
 * @count: number of refcounts to subtract.
 * @release: pointer to the function that will clean up
 * 	     when the last reference to the object is released.
 *
 * Subtract @count fro mthe refcount, and if 0, call release().
 * Return 1 if the object was removed, otherwise return 0.
 */
int ref_sub(struct ref *ref, int count, void (*release)(struct ref *ref));

/*
 * ref_put - decrement refcount for object.
 * @ref: object.
 * @release: pointer to the function that will clean up the object
 * 	     when the last reference to the object is released.
 * Decrement the refcount, and if 0, call release().
 * Return 1 if the object was removed, otherwise return 0.
 */
int ref_put(struct ref *ref, void (*release)(struct ref *ref));

#endif
