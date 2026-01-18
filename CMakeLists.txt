cmake_minimum_required(VERSION 3.16)

project(
    ISA_Calculator_Libary
    VERSION 1.0.0
    DESCRIPTION "ISA Calculator Library: standard atmosphere calculations"
    LANGUAGES CXX
)

# Create the library
add_library(ISA_Calculator_Libary
    src/ISA.cpp
)

# Header include paths
target_include_directories(ISA_Calculator_Libary
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Require C++17
target_compile_features(ISA_Calculator_Libary PUBLIC cxx_std_17)

# Optional compiler warnings
if (MSVC)
    target_compile_options(ISA_Calculator_Libary PRIVATE /W4)
else()
    target_compile_options(ISA_Calculator_Libary PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation (optional but useful)
install(TARGETS ISA_Calculator_Libary
    EXPORT ISA_Calculator_LibaryTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
