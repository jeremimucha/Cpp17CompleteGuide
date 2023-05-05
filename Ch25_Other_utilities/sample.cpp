#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

/**
 * C++17 introduced `std::sample` which outputs a randomized selection (a sample) for the given
 * range. It takes a pair of iterator indicating the range, a destination iterator, a number of
 * elements to sample and a random engine:
 */
template <typename InputIterator, typename OutputIterator, typename Distance,
          typename UniformRandomBitGenerator>
OutputIterator sample(InputIterator sourceBeg, InputIterator sourceEnd, OutputIterator destBeg,
                      Distance num, UniformRandomBitGenerator&& eng);

int main()
{
    std::vector<std::string> coll;
    std::generate_n(std::back_inserter(coll), 1000,
                    [i{0}]() mutable { return "value" + std::to_string(i++); });
    // Mersenne Twister engine
    std::random_device rd{};
    std::mt19937 engine{rd()};

    // destination range
    std::vector<std::string> dst;
    dst.resize(100);

    auto const dstend = std::sample(cbegin(coll), cend(coll),
                                    begin(dst),
                                    10,
                                    engine);
    std::cerr << "sampled elements:\n";
    std::for_each(std::begin(dst), dstend,
                  [](auto const& elem) { std::cerr << "\t" << elem << "\n"; });
}
