#include <iostream>
#include <vector>
#include <iterator>

/**
 * C++17 provides generic templates std::size(), std::empty() and std::data(), which return the
 * corresponding attribute of STL containers (including initializer_lists) and built-in arrays:
 * std::size - returns the size of a container/array
 * std::empty - informs if a container/array is empty
 * std::data - gives access to the raw data of a container
 */

int main()
{
    std::vector<int> v{};
    int arr[] = {9, 11, 13};
    std::initializer_list<double> il{3.14169265359, 2.71828182846};
    if (std::empty(v)) { std::cerr << "vector is empty\n"; }
    if (!std::empty(arr)) {
        std::cerr << "the array contains " << std::size(arr) << " elements: {";
        for (auto e : arr) { std::cerr << e << ", "; }
        std::cerr << "}\n";
    }
    if (!std::empty(il)) {
        std::cerr << "the initializer list contains " << std::size(il) << " elements: {"
        << std::data(il)[0] << ", " << std::data(il)[1] << "}\n";
    }
}
