#include <iostream>
#include <type_traits>
#include <utility>

/**
 * C++17 added `std::bool_constant` - a specialization of std::integral_constant, defined:
 * template<bool B>
 * using bool_constant = integral_constant<bool, B>;
 * using true_type = bool_constant<true>;
 * using false_type = bool_constant<false>;
 *
 * This makes defining boolean traits easier in some cases - one can derive from bool_constant
 * and include the condition in the template argument:
 */
template <typename T>
struct IsLargerThanIntT : std::bool_constant<(sizeof(int) < sizeof(T))> {
};
template <typename T>
constexpr static inline auto IsLargerThanInt{IsLargerThanIntT<T>::value};

// another example:
template <typename T>
struct IsNothrowMoveConstructibleT : std::bool_constant<noexcept(T(std::declval<T>()))> {
};

int main() {}
