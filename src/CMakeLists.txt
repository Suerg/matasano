include_directories(.)
if (UNIT_TEST)
	add_definitions(-DUNIT_TESTING)
endif (UNIT_TEST)
add_library(cryptlib cryptlib.c dataconvert.c datamanip.c score.c bytes.c ref.c
	bytes-node.c)
