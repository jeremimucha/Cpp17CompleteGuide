#include <array>
#include <iostream>
#include <type_traits>

/**
 * Since C++17 it is valid to use `auto` and `decltype(auto)` to specify a non-type template
 * parameter.
 */
template <auto N>
struct Foo {
};

Foo<42> fi;     // decltype(N) == int, N == 42
Foo<'a'> fch;   // decltype(N) == char, N == 'a'

// such templates can also be partially specialized:
template<unsigned N> struct Foo<N> { using type = unsigned; };

// `class template argument deduction` are also supported
template<typename T, auto N>
class Bar {
public:
    Bar(std::array<T, N> const&) { }
    Bar(T(&)[N]) { }
};

Bar b1{std::array<double, 10>{}};
Bar b2{"hello"};

// This feature also makes defining heterogenous or homogenous sequences easier:
template<auto... Vs> struct HeterogeousSequence { };
template<auto V, decltype(V)... Vs> struct HomogeneousSequence{ };

int main() {}
