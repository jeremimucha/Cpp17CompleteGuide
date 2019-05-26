#if !defined(ALIGNED_NEW_CPP11_HEADER_GUARD_)
#define ALIGNED_NEW_CPP11_HEADER_GUARD_

#include <cstdio>
#include <cstddef>  // std::size_t
#include <string>

#if __STDC_VERSION >= 201112L
#include <cstdlib>  // for aligned_alloc()
#else
#include <malloc.h>  // for _aligned_malloc() or memalign()
#endif

/**
 * Pre C++17 there was no support for allocating over-aligned data on the heap, it was necessary
 * to implement type-specific overloads for over-aligned data that handled the allocation.
 */

struct alignas(32) MyType32 {
    int i;
    char c;
    std::string s[4];

    // implement MyType32::operator new
    static void* operator new(std::size_t size)
    {
        printf("void* MyType32::operator new(std::size_t size)\n");
        // allocate memory for requested alignment:
#if __STDC_VERSION >= 201112L
        // use C++11 API
        return std::aligned_alloc(alignof(MyType32), size);
#else
#ifdef _MSC_VER
        // use Windows API
        return _aligned_malloc(size, alignof(MyType32));
#else
        // use Linux API
        return std::memalign(alignof(MyType32), size);
#endif
#endif
    }

    static void operator delete(void* p)
    {
        printf("void MyType32::operator delete(void* p)\n");
    // deallocate memory for requested alignment
#ifdef _MSC_VER
    // use Windows API
        _aligned_free(p);
#else
    // C++11 and Linux use generic free():
        free(p);
#endif
    }

    // since C++14:
    static void operator delete(void* p, std::size_t size)
    {
        printf("void MyType32::operator delete(void* p, std::size_t size)\n");
    // fallback on the non-size delete
        MyType32::operator delete(p);
    }
};

#endif  // ALIGNED_NEW_CPP11_HEADER_GUARD_
