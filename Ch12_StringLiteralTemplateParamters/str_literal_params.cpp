#include <iostream>
#include <type_traits>

// C++17 allows pointers with "no linkage" to be used as template parameters. This includes
// string literals:
template<char const* str>
struct Message { };

extern const char hello[]{"Hello World!"};      // external linkage
const char hello11[]{"Hello World!"};           // internal linkage

void foo()
{
    static const char hello17[]{"Hello World!"};    // no linkage

    Message<hello> msg;     // OK for all C++ versions
    Message<hello11> msg11; // OK since C++11
    Message<hello17> msg17; // OK since C++17
    Message<"Hello"> msg_;  // ERROR - invalid for all current C++ versions,
                            // might be OK in C++20
}

// This also solves another issue - it is now possible to use a compiletime function
// to specify an address (which wasn't valid pre C++17)
template<int* pint> struct Bar { };
int num{};
constexpr int* getIntPtr()
{
    return &num;
}
Bar<getIntPtr()> bar;   // valid since C++17

int main()
{

}
