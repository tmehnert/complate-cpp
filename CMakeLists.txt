# Copyright 2021 Torsten Mehnert
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
cmake_minimum_required(VERSION 3.14)
project(complate VERSION 0.0.1 DESCRIPTION "C++ library to render JSX-Components into HTML")

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_SHARED "Build shared library instead of static library" OFF)
option(USE_SANITIZER "Use sanitizer" OFF)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")

if (USE_SANITIZER)
    set(SANITIZER "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${SANITIZER}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${SANITIZER}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER}")
endif ()

include_directories(include)

add_subdirectory(lib/core)
add_subdirectory(lib/quickjs)
add_subdirectory(lib/v8)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()

if (BUILD_TESTS)
    include(cmake/Catch2.cmake)
    enable_testing()
    add_subdirectory(test/lib)
endif ()
