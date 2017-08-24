cmake_minimum_required(VERSION 3.6)
project(Lynx CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DLYNX_DEBUG=1 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG -DLYNX_DEBUG=0 -O3")

set(SOURCES
        source/lexer.cc
        source/lexer.h)
add_library(lynx_core STATIC ${SOURCES})
target_include_directories(lynx_core PUBLIC source)

add_executable(lynx source/main.cc)
target_include_directories(lynx PUBLIC source)
target_link_libraries(lynx lynx_core)

enable_testing()
find_package(GTest)
set(TESTS
        test/lexer_tests.cc
        test/main.cc)
add_executable(lynx_tests ${TESTS})
target_include_directories(lynx_tests PRIVATE source ${GTEST_INCLUDE_DIRS})
target_link_libraries(lynx_tests lynx_core ${GTEST_BOTH_LIBRARIES})
add_test(NAME lynx_tests COMMAND lynx_tests)

