cmake_minimum_required(VERSION 2.8)

project(Scheme)

set(DEFAULT_PROMPT "> " CACHE STRING "Default prompt")

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "-g -Wall -O2")
endif()

set(BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(BUILD_CONFIG_PATH ${BUILD_PATH}/config)

set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories ("${INCLUDE_PATH}")

## For config.h
include_directories ("${BUILD_CONFIG_PATH}")

set(BINARY sch)

include(CheckIncludeFiles)

check_include_files(unistd.h HAVE_UNISTD_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${BUILD_CONFIG_PATH}/config.h)


set(SOURCES
    ${SOURCE_PATH}/eval.c
    ${SOURCE_PATH}/hashtable.c
    ${SOURCE_PATH}/init.c
    ${SOURCE_PATH}/main.c
    ${SOURCE_PATH}/object.c
    ${SOURCE_PATH}/parser.c
    ${SOURCE_PATH}/primitive.c)

set(HEADERS
    ${INCLUDE_PATH}/eval.h
    ${INCLUDE_PATH}/hashtable.h
    ${INCLUDE_PATH}/init.h
    ${INCLUDE_PATH}/object.h
    ${INCLUDE_PATH}/parser.h
    ${INCLUDE_PATH}/primitive.h)

add_executable(${BINARY} ${SOURCES} ${HEADERS})
