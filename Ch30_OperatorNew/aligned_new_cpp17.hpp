#if !defined(ALIGNED_NEW_CPP17_HEADER_GUARD_)
#define ALIGNED_NEW_CPP17_HEADER_GUARD_

#include <cstddef>  // for std::size_t
#include <cstdio>
#include <cstdlib>  // for malloc(), aligned_alloc(), free()
#include <new>      // for std::align_val_t
#include <string>

/**
 * Since C++17 global new/delete overloads for over-aligned data are provided, it is now possible
 * to provide type-specific overloads for over-aligned data that delegate the work to the global
 * defaults.
 * It is also no longer necessary to provide type-specific overloads souly for the purpose of
 * handling over-aligned data allocation - the global overloads should work by default.
 */

struct alignas(32) MyType32 {
    int i;
    char c;
    std::string s[4];

    [[nodiscard]] static void* operator new(std::size_t size)
    {
        // called for default-aligned data
        printf("void* MyType32::operator new(std::size_t = %zd)\n", size);
        return ::operator new(size);
    }

    [[nodiscard]] static void* operator new(std::size_t size, std::align_val_t align)
    {
        // called for over-aligned data:
        printf("void* MyType32::operator new(std::size_t = %zd, std::align_val_t = %zd)\n", size,
               align);
        return ::operator new(size, align);
        // or call the C++11 aligned_alloc:
        // return std::aligned_alloc(static_cast<std::size_t>(align), size);
    }

    static void operator delete(void* p)
    {
        // called for default-aligned data
        printf("void MyType32::operator delete(void* = %p)\n", p);
        ::operator delete(p);
    }

    static void operator delete(void* p, std::size_t size)
    {
        printf("void MyType32::operator delete(void* = %p, std::size_t = %zd)\n", p, size);
        // could call the non-size class specific version (safer?)
        // MyType32::operator delete(p);
        ::operator delete(p, size);
    }

    static void operator delete(void* p, std::align_val_t align)
    {
        // called for over-aligned data
        printf("void MyType32::operator delete(void* = %p, std::align_val_t = %zd)\n", p, align);
        ::operator delete(p, align);
    }

    static void operator delete(void* p, std::size_t size, std::align_val_t align)
    {
        printf(
            "void MyType32::operator delete(void* = %p, std::size_t = %zd, std::align_val_t = %zd)\n",
            p, size, align);
        // could call the non-size class specific version instead (safer?)
        ::operator delete(p, size, align);
    }

    // same for arrays - new[], delete[] ...
};

#endif  // ALIGNED_NEW_CPP17_HEADER_GUARD_
