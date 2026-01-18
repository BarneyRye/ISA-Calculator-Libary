cmake_minimum_required(VERSION 3.16)

project(
    ISA_Calculator_Library
    VERSION 1.0.0
    DESCRIPTION "ISA Calculator Library"
    LANGUAGES CXX
)

add_library(ISA_Calculator_Library
    src/ISA.cpp
)

target_include_directories(ISA_Calculator_Library
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(ISA_Calculator_Library PUBLIC cxx_std_17)

# Compiler warnings (optional but recommended)
if (MSVC)
    target_compile_options(ISA_Calculator_Library PRIVATE /W4)
else()
    target_compile_options(ISA_Calculator_Library PRIVATE -Wall -Wextra -Wpedantic)
endif()
