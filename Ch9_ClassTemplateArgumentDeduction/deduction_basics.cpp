#include <iostream>
#include <vector>
#include <utility>

/**
 * C++17 introduced the ability to deduce class template arguments from the arguments passed
 * to the constructor.
 */

// By default the deduction is performed based on the declared constructor parameter types:
template <typename T>
struct Foo {
    Foo(T const&) {}
};
Foo f1{42}; // T = int
Foo f2{"hello deduction"};   // T = char[16];
// Note that the deduced type doesn't decay in this case - this is because references don't decay.

int main()
{
    // When deducing types if argument deduction could be interpreted as initializing a copy
    // the copy deduction is prefered:
    std::vector v1{42};     // std::vector<int>
    std::vector v2{v1};     // std::vector<int>
    static_assert(std::is_same_v<decltype(v1), decltype(v2)>);
    std::cerr << "v1.front() = " << v1.front() << "\n";
    std::cerr << "v2.front() = " << v2.front() << "\n";

    // class template argument deduction can now be used instead of convenience functions:
    auto p1 = std::make_pair(42, "hello");
    static_assert(std::is_same_v<decltype(p1), std::pair<int, char const*>>);
    // note that std::make_pair caused the char array argument to decay into `const char*`

    // The same can now be achieved by declaring std::pair directly:
    std::pair p2{42, "hello"};
    static_assert(std::is_same_v<decltype(p2), std::pair<int, char const*>>);
    // note that deduction also introduced decay here, although std::pair takes the arguments
    // by const&. This is achieved with `deduction guides` (see deduction_guides.cpp).
}
