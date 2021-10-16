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
set(BINARY ${CMAKE_PROJECT_NAME}-example)
find_package(Threads)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${SOURCES})
include_directories(${CMAKE_SOURCE_DIR}/deps/cpp-httplib)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} LINK_PUBLIC
        ${CMAKE_PROJECT_NAME}core
        ${CMAKE_PROJECT_NAME}quickjs
        ${CMAKE_THREAD_LIBS_INIT})

target_compile_definitions(${BINARY} PUBLIC TEST_RESOURCE_DIR="${CMAKE_SOURCE_DIR}/test/resources/")