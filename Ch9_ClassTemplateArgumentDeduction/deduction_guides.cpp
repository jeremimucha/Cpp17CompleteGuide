#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Deduction guides can be used to provide additional deduction guides or override the default
// ones.
template <typename T>
struct Foo {
    Foo(T const&, T const&) {}
};

// Without providing the deduction guides both arguments need to have the exact same type,
// this means that char arrays need to be of the exact same length:
Foo f1{"1234", "5678"}; // OK -> T = char[5]
// Foo f2{"123", "4567"};  // ERROR -> can't deduce T (conflicting types char[4] and char[5])

// ---
// This can be `fixed` with deduction guides - we could introduce decay, by deducing on
// by-value parameters, rather than by-const&:
template <typename T>
struct Bar {
    Bar(T const&, T const&) // ctor still takes by-const&
    {
    }
};

// deduction guides:
template <typename T>
Bar(T, T)->Bar<T>; // by-value causes built-in arrays to decay

Bar b1{"1234", "678"}; // T = const char*
static_assert(std::is_same_v<decltype(b1), Bar<const char*>>);

// ---
// Deduction guides don't need to be templates, and don't need to apply to constructors,
// they can be used for aggregate types as well
template <typename T>
struct Baz {
    T val;
};

Baz(char const*) -> Baz<std::string>;

// ---
// Overload resolution is used when choosing from deduction guides and constructors.
// For equally-well matching guides and ctors - deduction guides are prefered.
// Deduction guides can be marked `explicit` which makes them obay the same rules as marking
// constructors explicit does. This can be used to make overload resolution prefer constuctors
// for deduction over deduction guides in certain situations.

int main() {}
