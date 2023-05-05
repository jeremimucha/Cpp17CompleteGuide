#include <iostream>
#include <string>
#include <unordered_map>

/**
 * C++17 added `.try_emplace` member function to maps.
 * The new functions guarantee to only move their argument (if passed by rvalue ref) if the element
 * was actually emplaced - value for the key did not exist prior to the call.
 */

int main()
{
    std::unordered_map<int, std::string> foomap{{1, "foo"},
                                                {2, "bar"},
                                                {3, "baz"}};
    // before `.try_emplace` was introduced it was necessary to implement the following,
    // to guarantee that a move happened only if the element did not already exist:
    std::string s{"ni"};
    auto const pos = foomap.find(42);
    if (pos == foomap.cend()) {
        foomap.emplace(42, std::move(s));
    }
    // `.try_emplace` achieves the above, with the added benefit of looking up the position in
    // the map only once.
    std::string ss{"ninini"};
    auto const [it, res] = foomap.try_emplace(42, std::move(ss));
    if (res == false) {
        // if the element was not emplaced it is guaranteed that the string was not moved:
        std::cerr << "did not emplace value = \"" << ss << "\", element for key = "
            << 42 << " already exists\n";
    }
}
