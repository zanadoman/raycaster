# CMake
cmake_minimum_required(VERSION 3.25.1)
if(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/build)
  message(FATAL_ERROR "Incorrect build directory, CMake will exit.")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_CLANG_TIDY clang-tidy;-p;${CMAKE_BINARY_DIR})

# Project
project(game LANGUAGES C)

# Target
if(NOT ${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SYSTEM_NAME} MATCHES
  x86_64-Linux|AMD64-Windows)
  message(FATAL_ERROR "Unsupported target, CMake will exit.")
endif()

# Compiler
if(NOT ${CMAKE_C_COMPILER_ID} STREQUAL Clang)
  message(FATAL_ERROR "Unsupported compiler, CMake will exit.")
endif()
if(${CMAKE_C_COMPILER_VERSION} VERSION_LESS 18.0.3)
  message(FATAL_ERROR "Outdated compiler, CMake will exit.")
endif()

# Flags
file(READ ${CMAKE_SOURCE_DIR}/compile_flags.txt CMAKE_C_FLAGS)
string(REPLACE \n \  CMAKE_C_FLAGS ${CMAKE_C_FLAGS})

# Sources
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

# Libraries
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_mixer SDL2_ttf)

# Linker
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(LINK_OPTIONS -lmingw32 -mwindows)
endif()

# Assets
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/assets/)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/)
