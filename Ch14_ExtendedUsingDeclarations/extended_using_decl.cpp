#include <iostream>
#include <type_traits>
#include <string>

/**
 * C++17 extended `using` declarations to allow comma-separated list of declarations, this allows
 * them to be used in pack expensions.
 */

class Base {
public:
    void a();
    void b();
    void c();
};

class Derived : private Base {
public:
    using Base::a, Base::b, Base::c;
};

// ---
// One possible use case it to generically derive a function/operator from all base classes
template <typename... Ts>
struct overload : public Ts... {
    using Ts::operator()...;
};
// deduction guide
template <typename... Ts>
overload(Ts...)->overload<Ts...>;

// We can inherit `operator()` from any number of lambdas:
auto twice = overload{[](std::string& s) { s += s; }, [](auto& v) { v *= 2; }};

// ---
// Another use case - inheriting constructors from base classes:
template <typename T>
class BaseMulti {
    T value;
public:
    BaseMulti(T v) : value{v} {}
};

template <typename... Ts>
class Multi : private BaseMulti<Ts>... {
public:
    using BaseMulti<Ts>::BaseMulti...;
};

// This lets us use the inherited constructors - the remaining inherited-from classes are default
// initialized in the order of inheritance,
using MultiISB = Multi<int, std::string, bool>;
MultiISB m1{42};    // int = 42, std::string and bool default initialized
MultiISB m2{"hello"};   // int default initialized, std::string = "hello", bool default initialized

int main()
{
}
