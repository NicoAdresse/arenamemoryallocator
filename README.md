# Bare-Metal C++ Multiboot Kernel & Arena Allocator <->

A lightweight, freestanding C++ x86 kernel entry implementation featuring Multiboot 1 header specification compliance and high-performance arena memory allocation strategies.

---

## Overview <->

This project provides a bare-metal C++ entry point intended for x86 bootloaders like GRUB. It features zero-dependency memory management via fixed-alignment and dynamic-alignment arena allocators.

### Core Features <->

* **Multiboot 1 Specification Compliance**: Embeds a `.multiboot` section aligned to 32-bit boundaries for bootloader verification.
* **Freestanding C++ Core**: Designed to operate without standard OS runtime dependencies.
* **Arena Allocation Strategies**:
  * `ArenaAllocator8`: High-speed, fixed 8-byte alignment allocator for fixed-stride allocations.
  * `ArenaAllocatorDynamic`: Flexible power-of-two alignment allocator using standard `std::align` semantics with variadic template placement `new` support (`create<T>()`).

---

## How To Run <->

1. **Requirements**:
    Make sure you have these things instead: \
        A `C/C++` compiler (`GCC` is preferred). \
        `QEMU` \
        `CMake` 
2. **Run `MAKE.sh`**:
    As the name implies, run the `MAKE.sh` script. This will automatically create a new `build` \
    directory (if you haven't already deleted it) and automatically run CMake.
3. **Enjoy**: Enjoy.

---

## Project Structure <->

```text
ArenaAllocator
├── CMakeLists.txt
├── LICENSE
├── MAKE.sh
├── README.md
└── src
    └── main.cpp
```

## License <->

This project is under the [MIT LICENSE](./LICENSE).