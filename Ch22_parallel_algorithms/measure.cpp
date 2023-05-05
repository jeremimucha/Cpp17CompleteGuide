#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>
#include <iterator>
#include "timer.h"

struct Data {
    double value;
    double sqrt;
};

int main(int argc, char* argv[])
{
    int const num_elements = [argc, argv]() {
        if (argc > 1) {
            return std::atoi(argv[1]);
        }
        return 1000;
    }();

    std::vector<Data> coll;
    coll.reserve(num_elements);
    std::generate_n(std::back_inserter(coll), num_elements, [i{0}]() mutable noexcept {
        return Data{i++ * 3.14159265359, 0.0};
    });

    for (auto i{0}; i < 5; ++i) {
        auto const op = [](auto& val) noexcept { val.sqrt = std::sqrt(val.value); };
        Timer t;
        std::for_each(std::execution::seq,
                      std::begin(coll), std::end(coll),
                      op);
        t.print_diff("sequential: ");

        std::for_each(std::execution::par,
                      std::begin(coll), std::end(coll),
                      op);
        t.print_diff("parallel: ");
        std::cout << "\n";
    }

}
