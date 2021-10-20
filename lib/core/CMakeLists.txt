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
set(LIBRARY_NAME complatecore)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${SOURCES})

file(GLOB_RECURSE LIBRARY_INCLUDE_FILES "${CMAKE_SOURCE_DIR}/include/complate/core/*.h")

if (BUILD_SHARED)
    add_library(${LIBRARY_NAME} SHARED ${SOURCES})
else ()
    add_library(${LIBRARY_NAME} STATIC ${SOURCES})
endif ()

target_include_directories(${LIBRARY_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    )
target_link_libraries(${LIBRARY_NAME}
    PRIVATE "$<BUILD_INTERFACE:code_coverage>"
    )
set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${LIBRARY_INCLUDE_FILES}"
    )

install(TARGETS ${LIBRARY_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/complate/core
    )
