cmake_minimum_required(VERSION 2.8)

project(Scheme)

set(BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(TEST_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_BINARY cmd_test)

set(TEST_SOURCES
    ${TEST_SOURCE_PATH}/cmd_test.c)

add_executable(${TEST_BINARY} ${TEST_SOURCES})
