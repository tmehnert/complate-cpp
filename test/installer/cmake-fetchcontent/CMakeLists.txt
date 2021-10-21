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
Include(FetchContent)

project(complate-cmake-fetchcontent DESCRIPTION "Test import complate using CMake FetchContent")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

message("-- Fetching complate...")
if (NOT COMPLATE_GIT_TAG)
    set(COMPLATE_GIT_TAG HEAD)
endif ()
FetchContent_Declare(
    complate
    GIT_REPOSITORY https://github.com/tmehnert/complate-cpp.git
    GIT_TAG ${COMPLATE_GIT_TAG}
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(complate)
message("-- Fetching complate - done")

add_executable(${PROJECT_NAME} main.cpp)
add_test(NAME "CMakeInstallUsingFetchContent" COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE complate::quickjs complate::v8)
