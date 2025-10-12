cmake_minimum_required(VERSION 3.15)
project(MobVacuum VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    # For Microsoft Visual C++
    add_compile_options(/W4 /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # For GCC and Clang
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O2 -DNDEBUG)
    endif()
endif()

# Enable AVX/SSE support for SIMD operations
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-msse4.1 -mavx -mavx2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/arch:AVX2)
endif()

# Source files
set(SOURCES
    mob_vacuum.cpp
)

set(HEADERS
    mob_vacuum.h
)

# Create the main library
add_library(mob_vacuum STATIC ${SOURCES} ${HEADERS})

# Set include directories
target_include_directories(mob_vacuum PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries (platform-specific)
if(WIN32)
    # Windows-specific libraries if needed
    # target_link_libraries(mob_vacuum PRIVATE user32 kernel32)
elseif(UNIX)
    # Unix-specific libraries if needed
    target_link_libraries(mob_vacuum PRIVATE pthread)
endif()

# Installation rules
install(TARGETS mob_vacuum
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include
)

# Optional: Create a simple test executable
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples OPTIONAL)
endif()

# Optional: Enable testing
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests OPTIONAL)
endif()