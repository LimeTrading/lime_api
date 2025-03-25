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



include(FetchContent)

###############################################################################
function(fetch_dependency dependency)

    list(GET dependency 0 url)
    list(LENGTH dependency arity)
    if (arity GREATER 1)
        list(GET dependency 1 tag)
    else()
        set(tag, "")
    endif()
    if ("${tag}" STREQUAL "")
        set(tag "${GIT_BRANCH}") # if no branch specified then use current branch
    endif()
    string(REPLACE "/" ";" dependency "${url}")
    list(POP_BACK dependency repo_name_with_ext)
    string(REPLACE "." ";" R "${repo_name_with_ext}")
    list(POP_FRONT R repo_name)

    string(ASCII 27 Esc)
    set(ColorReset "${Esc}[m")
    set(ColorBold  "${Esc}[1m")
    set(ColorRed  "${Esc}[31m")

    set(USING_CURRENT_BRANCH 1)
    if (NOT "${tag}" STREQUAL "${GIT_BRANCH}")
        set(USING_CURRENT_BRANCH 0)
    endif()

    message("\n${ColorBold}===================================================================================================================${ColorReset}")
    message("${ColorBold}Fetching Content:${ColorReset} REPOSITORY = ${repo_name}, URL = ${url}, BRANCH = ${tag}  ")
    if (NOT USING_CURRENT_BRANCH)
        message("${ColorRed}${ColorBold}Warning: fetching branch ${tag} instead of current branch ${GIT_BRANCH}")
    endif()
    message("${ColorBold}===================================================================================================================${ColorReset}")

    FetchContent_Declare(
        ${repo_name}
        GIT_REPOSITORY "${url}"
        GIT_TAG ${tag}
        SOURCE_DIR        "${CMAKE_BINARY_DIR}/${repo_name}-src"
        BINARY_DIR        "${CMAKE_BINARY_DIR}/${repo_name}-build"
        INSTALL_DIR       "${CMAKE_BINARY_DIR}"
        INSTALL_COMMAND   ""
        )
    FetchContent_MakeAvailable(${repo_name})
    FetchContent_GetProperties(${repo_name})

    set(_${repo_name}_src_path ${CMAKE_BINARY_DIR}/${repo_name}-src CACHE STRING "")
endfunction()
