cmake_minimum_required(VERSION 3.2)
add_definitions("@/home/suerg/.gcc")
option(UNIT_TEST "UNIT_TEST" OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
project(cryptlib)
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
file(COPY res DESTINATION build/res)
add_custom_target(copy-files ALL
	    COMMAND cmake -E copy_directory
	    ${CMAKE_SOURCE_DIR}/res
	    ${CMAKE_BINARY_DIR}/res
	    )
