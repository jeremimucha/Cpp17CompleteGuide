#include <array>
#include <iostream>
#include <type_traits>

/**
 * Since C++17 lambdas are implicitly constexpr.
 * Using any non-constexpr features (e.g. static local variables, virtual calls, try-catch blocks)
 * within the lambda disables using the lambda in a compiletime-context.
 */

int main()
{
    // implicitly constexpr
    auto const squared1 = [](auto val) noexcept { return val * val; };
    std::array<int, squared1(5)> arr25{};

    // explicit constexpr declaration
    auto const squared2 = [](auto val) constexpr noexcept { return val * val; };
    std::array<int, squared2(3)> arr9{};

    // using non-constexpr features in a explicitly constexpr lambda results
    // in a compiletime error
    // auto const squared3 = [](auto val) constexpr {
    //     static int calls{0}; // ERROR
    //     ++calls;
    //     return val * val;
    // };
}
