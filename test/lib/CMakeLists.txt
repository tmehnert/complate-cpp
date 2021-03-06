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
set(BINARY ${CMAKE_PROJECT_NAME}-tests)
find_package(Threads)
include(Catch)

add_executable(${BINARY} main.cpp)
add_subdirectory(fixtures)
add_subdirectory(core)
add_subdirectory(quickjs)
if (BUILD_V8_RENDERER)
    add_subdirectory(v8)
    target_compile_definitions(${BINARY} PRIVATE COMPLATE_V8_INCLUDED)
endif()

target_link_libraries(${BINARY} PUBLIC
    Catch2::Catch2
    trompeloeil
    ${CMAKE_THREAD_LIBS_INIT}
    PRIVATE "$<BUILD_INTERFACE:code_coverage>"
    )
target_compile_definitions(${BINARY}
    PUBLIC TEST_RESOURCE_DIR="${CMAKE_SOURCE_DIR}/test/resources/"
    )

catch_discover_tests(${BINARY})
