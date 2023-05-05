#include <iostream>
#include <type_traits>
#include <utility>

/**
 * C++17 standarized `std::void_t` - it can be used in unevaluated contexts to make use of SFINAE,
 * when defining type-traits.
 */

template <typename T, typename = std::void_t<>>
struct HasVariousT : std::false_type {
};

template <typename T>
struct HasVariousT<T,
    std::void_t<decltype(std::declval<T>().begin()),
                typename T::difference_type,
                typename T::iterator>>
    : std::true_type {
};

int main()
{

}

