#include <iostream>
#include <unordered_map>
#include <string>

/**
 * C++17 added `.insert_or_assign()` to associative containers - it guarantees that the value is
 * moved either to new or an existing element.
 */

int main()
{
    std::unordered_map<int, std::string> foomap{{1, "foo"},
                                                {2, "bar"},
                                                {3, "baz"}};
    // before `.insert_or_assign()`
    std::string ni{"ni"};
    auto const pos = foomap.find(42);
    if (pos == foomap.cend()) {
        foomap.emplace(42, std::move(ni));
    } else {
        pos->second = std::move(ni);
    }
    // which would be equivalent to
    // foomap[42] = std::move(s);   // overhead of default constructing the element
                                    // and then assigning to it

    // All this can now be cleanly and more efficiently done with `.insert_or_assign()`
    std::string ni3{"ninini"};
    foomap.insert_or_assign(42, std::move(ni3));    // note it takes the key and value
                                                    // as separate arguments
}
