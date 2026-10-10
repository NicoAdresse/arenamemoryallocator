// src/main.cpp

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <cstring>
#include <utility>

#define MULTIBOOT_SEARCH            8192
#define MULTIBOOT_HEADER_MAGIC      0x1BADB002
#define MULTIBOOT_HEADER_FLAGS      0x00000003
#define MULTIBOOT_CHECKSUM          (0u - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS))

struct MultibootHeader
{
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
};

[[gnu::section(".multiboot"), gnu::used, gnu::aligned(4)]]
const MultibootHeader multiboot_header = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    MULTIBOOT_CHECKSUM
};

// 8 Byte Only
class ArenaAllocator8
{
private:
    uint8_t* buffer;
    size_t capacity;
    size_t offset;
public:
    explicit ArenaAllocator8(void* backingBuffer, size_t bufferSize)
        : buffer(reinterpret_cast<uint8_t*>(backingBuffer)), capacity(bufferSize), offset(0) {}
    ~ArenaAllocator8() { arenaReset(); }

    ArenaAllocator8(const ArenaAllocator8&) = delete;
    ArenaAllocator8& operator=(const ArenaAllocator8&) = delete;
    ArenaAllocator8(ArenaAllocator8&&) noexcept = default;
    ArenaAllocator8& operator=(ArenaAllocator8&&) noexcept = default;
    
    // Allocates memory
    auto arenaAlloc(size_t size) -> void*
    {
        uintptr_t currentPtr { reinterpret_cast<uintptr_t>(buffer) + offset };
        uintptr_t alignedPtr { (currentPtr + 7) & ~static_cast<uintptr_t>(7) };
        size_t padding { alignedPtr - currentPtr };

        if (size > capacity || offset > capacity - size - padding)
        {
            return nullptr;
        }

        offset += padding + size;
        return reinterpret_cast<void*>(alignedPtr);
    }

    // Resets memory
    auto arenaReset() -> void
    {
        offset = 0;
    }
};

// Supports Multiple Bytes
class ArenaAllocatorDynamic
{
private:
    uint8_t* buffer;
    size_t capacity;
    size_t offset;

    static constexpr auto isPowerOfTwo(size_t x) noexcept -> bool
    {
        return (x != 0) && ((x & (x - 1)) == 0);
    }
public:
    explicit ArenaAllocatorDynamic(void* backingBuffer, size_t bufferSize)
        : buffer(reinterpret_cast<uint8_t*>(backingBuffer)), capacity(bufferSize), offset(0) {}
    ~ArenaAllocatorDynamic() { arenaReset(); }

    ArenaAllocatorDynamic(const ArenaAllocatorDynamic&) = delete;
    ArenaAllocatorDynamic& operator=(const ArenaAllocatorDynamic&) = delete;
    ArenaAllocatorDynamic(ArenaAllocatorDynamic&&) noexcept = default;
    ArenaAllocatorDynamic& operator=(ArenaAllocatorDynamic&&) noexcept = default;

    // Allocates memory
    auto arenaAlloc(size_t size, size_t alignment) -> void*
    {
        if (!isPowerOfTwo(alignment)) return nullptr;
        if (offset > capacity) return nullptr;

        size_t remainingSpace { capacity - offset };
        void* ptr { static_cast<void*>(buffer + offset) };

        if (!std::align(alignment, size, ptr, remainingSpace))
        {
            return nullptr;
        }

        uint8_t* alignedPtr { static_cast<uint8_t*>(ptr) };
        offset = static_cast<size_t>(alignedPtr - buffer) + size;

        return ptr;
    }

    // Allocates memory, pads it with 0s
    auto arenaAllocZeroed(size_t size, size_t alignment) -> void*
    {
        void* ptr { arenaAlloc(size, alignment) };

        if (ptr)
        {
            std::memset(ptr, 0, size);
        }

        return ptr;        
    }

    // Uses templates to allocate memory for non-trivial types
    template <typename T, typename... Args>
    auto create(Args&&... args) -> T*
    {
        void* rawMem { arenaAlloc(sizeof(T), alignof(T)) };
        if (!rawMem) 
        {
            return nullptr;
        }

        return ::new (rawMem) T(std::forward<Args>(args)...);
    }

    // Resets memory
    auto arenaReset() -> void
    {
        offset = 0;
    }
};

// Buffer
static uint8_t buffer[15000];

// Starting point
extern "C" auto allocatorStart() -> void
{
    auto allocator { ArenaAllocatorDynamic(buffer, sizeof(buffer)) };
    [[maybe_unused]] int* myNumber { allocator.create<int>() };

    while (true)
    {
        asm volatile("hlt");
    }
}