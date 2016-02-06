#ifndef UNIT_TEST_H
#define UNIT_TEST_H

/*
 * This is enabled or disabled by passing -DUNIT_TEST=ON to cmake.
 * gotcha: make sure to delete CMakeCache.txt first.
 */

#ifdef UNIT_TESTING

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#undef assert
#define assert(expression) \
	mock_assert((int)(expression), (#expression), __FILE__, __LINE__)


#endif

#endif
