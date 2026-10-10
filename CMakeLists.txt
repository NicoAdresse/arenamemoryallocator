cmake_minimum_required(VERSION 3.10.0)
project(arena_memory_allocator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(arena_allocator_exec src/main.cpp)

target_compile_options(arena_allocator_exec PRIVATE
    -m32
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -Wall
    -Wextra
)

target_link_options(arena_allocator_exec PRIVATE
    -m32
    -nostdlib
    -e allocatorStart
    -Wl,--section-start=.multiboot=0x100000
)

add_custom_target(run
    COMMAND qemu-system-x86_64 -kernel $<TARGET_FILE:arena_allocator_exec>
    DEPENDS arena_allocator_exec
)
