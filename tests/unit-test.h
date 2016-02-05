#ifndef UNIT_TEST_H
#define UNIT_TES_H

#ifdef UNIT_TESTING

#define malloc(size) (test_malloc((size)))
#define calloc(num, size) (test_calloc((num), (size)))
#define free(ptr) (test_free((ptr)))
#define realloc(ptr, size) (test_realloc((ptr), (size)))

#endif

#endif
