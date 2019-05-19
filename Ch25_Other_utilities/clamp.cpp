#include <algorithm>
#include <iostream>

/**
 * C++17 introduces `std::clamp` which imposes min and max requirements on a value (clamps).
 * The effects are the same as calling `std::min(std::max(value, some_min), some_max)`
 *   template<typename T>
 *   constexpr const T& clamp(const T& value, const T& min, const T& max);
 * 
 */

int main()
{
    for (int i : {-7, 0, 8, 15}) {
        std::cout << std::clamp(i, 5, 13) << '\n';
    }
    // Optionally it accepts a predicate:
    std::cerr << "\nWith predicate:\n";
    for (int i : {-7, 0, 8, 15}) {
        std::cout << std::clamp(i, 5, 13,
                                [](auto l, auto r){ return std::abs(l) < std::abs(r); })
            << '\n';
    }
}
