#include <iostream>
#include <type_traits>

/**
 * C++17 fixed inconsistent behavior for direct list initialization of `auto` variables.
 */

// Pre C++17:
// int x{42}; // initializes an int
// int y{1,2,3}; // ERROR
// auto a{42}; // initializes a std::initializer_list<int>
// auto b{1,2,3}; // OK: initializes a std::initializer_list<int>

// Since C++17
int x{42}; // initializes an int
// int y{1,2,3}; // ERROR
auto a{42}; // initializes an int now
// auto b{1,2,3}; // ERROR now
static_assert(std::is_same_v<decltype(a), int>);

// assignment/copy list initialization now consistently initializes to std::initialized_list:
auto il1 = {42};
auto il2 = {1, 2, 3};
static_assert(std::is_same_v<decltype(il1), std::initializer_list<int>>);
static_assert(std::is_same_v<decltype(il2), std::initializer_list<int>>);

int main()
{

}
