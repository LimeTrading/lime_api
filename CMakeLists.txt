# MIT License
# 
# Copyright (c) 2025 Lime Trading
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# Contributors: MAM
# Creation Date:  March 25th, 2025

cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 20)

###############################################################################
# get this git repository name
###############################################################################
execute_process(
    COMMAND git rev-parse --show-toplevel
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE X
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND basename ${X}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_RESPOSITORY
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(PROJECT_NAME lime_api)
project(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -g
        -O0
        -Wall
        -fPIC
    )
else()
    add_compile_options(
        -g
        -O2
        -march=native
        -Wall
        -fPIC
    )
endif()


###############################################################################
include("./scripts/fetch_dependencies.cmake")
# fetch_dependency("git@github:account/repo_name;branch")

set(CMAKE_INSTALL_LIBDIR /usr/lib64)
set(_${PROJECT_NAME}_dir ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(./public/src)
