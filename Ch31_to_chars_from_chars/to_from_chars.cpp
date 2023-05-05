#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "to_from_chars.hpp"

int main()
{
    std::vector<double> coll{3.14159265359, 2.71828182846, 0.00001};

    // create two slightly different floating-point values by accumulating in a different order
    auto const sum1 =
        std::accumulate(coll.cbegin(), coll.cend(), 0.0, [](auto a, auto b) { return a + b; });
    auto const sum2 =
        std::accumulate(coll.crbegin(), coll.crend(), 0.0, [](auto a, auto b) { return a + b; });

    // appear to be the same
    std::cerr << "sum1: " << sum1 << "\n";
    std::cerr << "sum2: " << sum2 << "\n";

    // but are different
    std::cerr << std::boolalpha << std::setprecision(20);
    std::cerr << "sum1 == sum2 -> " << (sum1 == sum2) << "\n";  // false

    std::cerr << "sum1: " << sum1 << "\n";
    std::cerr << "sum2: " << sum2 << "\n";
    std::cerr << "\n";

    d2str2d(sum1);
    d2str2d(sum2);

// Output on MSVC 19.20:
// sum1: 5.85988
// sum2: 5.85988
// sum1 == sum2 -> false
// sum1: 5.8598844820499991215
// sum2: 5.8598844820500000097

// input: 5.8598844820499991215
// str: 5.859884482049999
// input: 5.8598844820500000097
// str: 5.85988448205
}
