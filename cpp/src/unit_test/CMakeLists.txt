cmake_minimum_required(VERSION 2.6)
project (unit_test)
set(Boost_USE_STATIC_LIBS ON)

find_package( Boost REQUIRED )

include_directories(../../include ${Boost_INCLUDE_DIRS})
set (UNIT_TEST_SRCS unit_test.cpp)
if (WIN32)
	set (UNIT_TEST_SRCS ${UNIT_TEST_SRCS}
		unit_test_dictionary.hpp
		unit_test_k_shortest_path.hpp
		unit_test_segment.hpp
		unit_test_serialization.hpp
		unit_test_utility.hpp
		unit_test_viterbi.hpp)
endif (WIN32)

if (UNIX)
	set (CMAKE_EXE_LINKER_FLAGS -lboost_iostreams)
endif (UNIX)

add_executable (unit_test ${UNIT_TEST_SRCS})
target_link_libraries (unit_test ${Boost_LIBRARY_DIRS})

