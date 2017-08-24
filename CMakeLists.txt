cmake_minimum_required(VERSION 3.6)
project(Lynx CXX)

#set(SOURCES <sources_here>)
#add_library(lynx_core STATIC ${SOURCES})

add_executable(lynx source/main.cc)
target_include_directories(lynx PUBLIC source)
#target_link_libraries(lynx lynx_core)

find_package(GTest)

enable_testing()
set(TESTS test/main.cc)
add_executable(lynx_tests ${TESTS})
target_include_directories(lynx PRIVATE source ${GTEST_INCLUDE_DIRS})
target_link_libraries(lynx_tests ${GTEST_BOTH_LIBRARIES})
add_test(NAME lynx_tests COMMAND lynx_tests)

