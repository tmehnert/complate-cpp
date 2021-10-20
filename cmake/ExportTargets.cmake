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
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(VERSION_CONFIG_FILE "${GENERATED_DIR}/${CMAKE_PROJECT_NAME}-config-version.cmake")
set(PROJECT_CONFIG_FILE "${GENERATED_DIR}/${CMAKE_PROJECT_NAME}-config.cmake")

configure_package_config_file(
    cmake/ComplateConfig.cmake.in
    "${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)
write_basic_package_version_file(
    "${VERSION_CONFIG_FILE}" COMPATIBILITY ExactVersion
)

install(
    FILES "${PROJECT_CONFIG_FILE}" "${VERSION_CONFIG_FILE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${CMAKE_PROJECT_NAME}::"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
