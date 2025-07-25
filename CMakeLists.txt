cmake_minimum_required(VERSION 3.28)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    Broadercast
    VERSION 1.0.0.0
    LANGUAGES CXX
)

# cache variables for installation destinations
include(GNUInstallDirs)

add_library(BroadercastFacilities INTERFACE)

set_target_properties(BroadercastFacilities PROPERTIES
    OUTPUT_NAME BroadercastFacilities
    EXPORT_NAME Facilities
)

target_include_directories(BroadercastFacilities INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


if(CMAKE_SOURCE_DIR STREQUAL Broadercast_SOURCE_DIR OR TEST_Broadercast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestBroadercast
    ${CMAKE_CURRENT_LIST_DIR}/tests/tests.cpp
)


target_include_directories(TestBroadercast
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

find_package(Conceptrodon REQUIRED CONFIG)
target_link_libraries(TestBroadercast PRIVATE Conceptrodon::Facilities)

endif()


install(
    TARGETS BroadercastFacilities
    EXPORT Broadercast
)

install(
    DIRECTORY Broadercast
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT Broadercast
    FILE Broadercast.cmake
    NAMESPACE Broadercast::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Broadercast
)

# include the package config helpers and generate the file below
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/BroadercastConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Broadercast
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/BroadercastConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Broadercast
)