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
file(READ "VERSION" VERSION_FILE)
string(STRIP ${VERSION_FILE} VERSION_FILE)
project(complate VERSION ${VERSION_FILE} DESCRIPTION "C++ library to render JSX-Components into HTML")
include(cmake/CodeCoverage.cmake)

set(EXCLUDE_FROM_ALL_ON_IMPORT EXCLUDE_FROM_ALL)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(EXCLUDE_FROM_ALL_ON_IMPORT "")
endif ()

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_SHARED "Build shared library instead of static library" OFF)
option(BUILD_V8_RENDERER "Build V8 renderer" ON)
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

if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-mbig-obj")
endif ()

if (USE_SANITIZER)
    set(SANITIZER "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${SANITIZER}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${SANITIZER}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER}")
endif ()

set(TARGETS_EXPORT_NAME "${CMAKE_PROJECT_NAME}-targets")
add_subdirectory(lib/core ${EXCLUDE_FROM_ALL_ON_IMPORT})
add_subdirectory(lib/quickjs ${EXCLUDE_FROM_ALL_ON_IMPORT})
if (BUILD_V8_RENDERER)
    add_subdirectory(lib/v8 ${EXCLUDE_FROM_ALL_ON_IMPORT})
endif ()
include(cmake/ExportTargets.cmake)

if (BUILD_EXAMPLE)
    include(cmake/CppHttplib.cmake)
    add_subdirectory(example ${EXCLUDE_FROM_ALL_ON_IMPORT})
endif ()

if (BUILD_TESTS)
    include(cmake/Catch2.cmake)
    include(cmake/Trompeloeil.cmake)
    enable_testing()
    add_subdirectory(test/lib ${EXCLUDE_FROM_ALL_ON_IMPORT})
endif ()
