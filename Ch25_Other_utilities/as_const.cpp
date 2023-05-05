#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

/**
 * C++17 adds `std::as_const` utility template - it forwars the parameter as a const reference.
 * It can be used when passing otherwise non-const variables in a context where they should
 * behave as const - e.g. if we'd like to force the deduced type to be const, or when
 * capturing lambda references.
 */

int main()
{
    std::string mutable_string{"Hello as_const"};
    auto const lambda = [&str=std::as_const(mutable_string)]() {
        std::cerr << "The captured reference is const: " << std::boolalpha
            << std::is_const_v<std::remove_reference_t<decltype(str)>> << "\n";
    };
    std::cerr << "main() scope - the string is non-const: " << std::boolalpha
        << !std::is_const_v<decltype(mutable_string)> <<"\n";
    lambda();
}
