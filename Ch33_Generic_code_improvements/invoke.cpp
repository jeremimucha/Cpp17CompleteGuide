#include <iostream>
#include <functional>
#include <utility>
#include <vector>

/**
 * C++17 added `std::invoke` - a utility that allows to call any callable with the given arguments.
 * It correctly handles pointers to member functions by treating the first argument after the
 * callable itself as the object to invoke the callable on.
 * A number of related type-traits are also defined:
 * `std::invoke_result`  - the resulting type of invoking a callable `C` with arguments `Args`
 * `std::is_invokable`   - bool trait informing if a given type is invokable (callable)
 * `std::is_invokable_r` - same, but also considers the return type
 */

void print(std::vector<int> const& coll)
{
    std::cout << "elements: ";
    for (auto const& e : coll) {
        std::cout << e << ", ";
    }
    std::cout << "\n";
}

int main()
{
    std::vector<int> vals{0, 8, 15, 23, 42, -1, 99};

    std::invoke([](auto const& coll){std::cout << "coll size = " << coll.size() << "\n";}, vals);
    std::invoke(print, vals);

    std::invoke(&decltype(vals)::pop_back, vals);
    std::invoke(print, vals);

    // calling an overloaded function requires specifying exactly which overload to call
    std::invoke<void(decltype(vals)::*)(std::size_t)>(&decltype(vals)::resize, vals, 5);
    std::invoke(print, vals);

    std::invoke(&decltype(vals)::clear, vals);
    std::invoke(print, vals);
}
