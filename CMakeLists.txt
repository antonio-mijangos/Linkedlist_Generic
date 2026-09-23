cmake_minimum_required(VERSION 3.25)

# ---------------------------------------------------------------------------
# Project information
# ---------------------------------------------------------------------------

project(
    MyProject
    VERSION 1.0.0
    DESCRIPTION "Professional template for C"
    LANGUAGES C
)

include(CTest)


# ---------------------------------------------------------------------------
# C Standard configuration
# ---------------------------------------------------------------------------

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Generate compile_commands.json for VS Code IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ---------------------------------------------------------------------------
# Library configuration
# ---------------------------------------------------------------------------

set(LIBRARY_NAME linked_list)


# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------

set(SOURCES
    src/linked_list.c
)


# ---------------------------------------------------------------------------
# Create static library
# ---------------------------------------------------------------------------

add_library(
    ${LIBRARY_NAME}
    STATIC
    ${SOURCES}
)

# ---------------------------------------------------------------------------
# Test executable
# ---------------------------------------------------------------------------

set(TEST_NAME linked_list_test)

add_executable(
    ${TEST_NAME}
    test/test_linked_list.c
)

target_link_libraries(
    ${TEST_NAME}
    PRIVATE
        ${LIBRARY_NAME}
)

add_test(
    NAME linked_list_tests
    COMMAND ${TEST_NAME}
)


# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------

target_include_directories(
    ${LIBRARY_NAME}

    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


# ---------------------------------------------------------------------------
# Compiler warnings and options
# ---------------------------------------------------------------------------

if(MSVC)

    target_compile_options(
        ${LIBRARY_NAME}

        PRIVATE
            /W4
            /permissive-
            /utf-8
    )

else()

    target_compile_options(
        ${LIBRARY_NAME}

        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

endif()


# ---------------------------------------------------------------------------
# Output directories
# ---------------------------------------------------------------------------

set_target_properties(
    ${LIBRARY_NAME}

    PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/lib
)



