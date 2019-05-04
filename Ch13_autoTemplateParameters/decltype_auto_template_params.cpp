#include <iostream>
#include <type_traits>

/**
 * It is also valid to declare non-type template parameters using `decltype(auto)`, which will
 * cause the parameter type to be deduced using `decltype(auto)` rules:
 * - type for a prvalue (e.g., temporaries)
 * - type& for an lvalue (e.g., objects with names)
 * - type&& for an xvalue (e.g., objects casted to rvalue-references, as with std::move()).
 */

template<decltype(auto) N>
struct Foo {
    void printN() const {
        std::cout << __PRETTY_FUNCTION__ << ", N = " << N << "\n";
    }
};

static constexpr int const c{42};
static int v{42};

int main()
{
    Foo<c> f1;      // N -> int const
    Foo<(c)> f2;    // N -> int const&
    f1.printN();
    f2.printN();

    Foo<(v)> f3;   // N -> int&
    v = 77;
    f3.printN();   // prints N = 77
}
