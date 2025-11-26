cmake_minimum_required(VERSION 3.15)
project(fstring VERSION 2.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options
if(MSVC)
    add_compile_options(/W4 /permissive-)
    # Enable constexpr evaluation
    add_compile_options(/constexpr:depth1024)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Enable constexpr evaluation
    add_compile_options(-fconstexpr-depth=1024)
endif()

# Header-only library interface
add_library(fstring INTERFACE)
target_include_directories(fstring INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional: enable concepts checking
target_compile_features(fstring INTERFACE cxx_std_20)

# Examples executable
add_executable(fstring_examples src/examples.cpp)
target_link_libraries(fstring_examples PRIVATE fstring)

# Test executable
add_executable(fstring_tests src/test.cpp)
target_link_libraries(fstring_tests PRIVATE fstring)

# Enable testing
enable_testing()
add_test(NAME fstring_unit_tests COMMAND fstring_tests)

# Installation
include(GNUInstallDirs)

install(TARGETS fstring
    EXPORT fstringTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT fstringTargets
    FILE fstringTargets.cmake
    NAMESPACE zuu::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fstring
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fstringConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/fstringConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fstring
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/fstringConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/fstringConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/fstringConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fstring
)

# Documentation (optional, requires Doxygen)
option(BUILD_DOCUMENTATION "Build documentation using Doxygen" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_MAN NO)
        
        doxygen_add_docs(docs
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            COMMENT "Generate API documentation"
        )
    endif()
endif()

# Print configuration
message(STATUS "fstring Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
