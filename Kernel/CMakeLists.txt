cmake_minimum_required(VERSION 3.4.1)
project(atrikrnl)

set(TARGET_ARCH "x86_64")

file(GLOB_RECURSE KERNEL_SOURCES
        src/Device/*.cpp
        src/Drivers/*.cpp
        src/Fs/*.cpp
        src/Input/*.cpp
        src/Memory/*.cpp
        src/Misc/*.cpp
        src/Net/*.cpp
        src/Process/*.cpp
        src/Video/*.cpp
        src/Runtime.cpp
        src/String.cpp
        src/System.cpp)

add_compile_options(-Wall -fno-exceptions -fno-rtti)
add_compile_options(-Wno-write-strings -Wno-unused-parameter -Wno-sign-compare -Wno-deprecated-volatile)
add_compile_options(-ffreestanding -finline-functions -nostdlib)
add_compile_options(-mno-mmx -mno-sse -mno-sse2)
add_compile_options(-mcmodel=large -mno-red-zone -fno-pic)
add_compile_options(-O0)

include_directories(include)

if ("${TARGET_ARCH}" STREQUAL "x86_64")
    include_directories(include/Arch/x86_64)
    enable_language(ASM_NASM)
else()
    message(FATAL_ERROR "Unsupported architecture ${TARGET_ARCH}")
endif()

