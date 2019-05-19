#include <iostream>
#include <map>
#include <string>

/**
 * C++17 allows to splice Nodes in and out of associative containers - maps and sets.
 * This allows for moving/merging nodes between containers as well as mutating node keys
 * without the need for allocation. Until C++17 it was necessary to remove and insert a node
 * if they key was to be modified.
 */

template<typename T>
void print_map(T const& coll)
{
    std::cerr << "[ ";
    for (const auto& [key, value] : coll) {
        std::cerr << "{" << key << ": " << value << "}, ";
    }
    std::cerr << "]\n";
}

int main()
{
    std::map<int, std::string> foomap{{1, "foo"},
                                      {2, "bar"},
                                      {3, "baz"}};
    std::cerr << "map:      ";
    print_map(foomap);
    auto node_handle = foomap.extract(2);    // node_handle has type decltype(m)::node_type
    node_handle.key() = 4;
    foomap.insert(std::move(node_handle));
    std::cerr << "after extracting, modifying and reinserting the node:\nmap:      ";
    print_map(foomap);

    std::multimap<int, std::string> othermap{{1, "one"},
                                             {2, "two"},
                                             {3, "three"},
                                             {4, "four"}};
    std::cerr << "\n\nmultimap: ";
    print_map(othermap);

    // move elements between containers
    othermap.insert(foomap.extract(foomap.find(3))); // extract and insert using iterators
    othermap.insert(foomap.extract(4));              // extract and insert using key/node_type directly
    std::cerr << "after moving elements from map to multimap via extracting nodes:\n";
    std::cerr << "map:      ";
    print_map(foomap);
    std::cerr << "multimap: ";
    print_map(othermap);

    // Thanks to the node-extract interface associative containers support merging now:
    // .merge moves all elements from one container to another (which don't already exist)
    foomap.merge(othermap);
    std::cerr << "map:      ";
    print_map(foomap);
    std::cerr << "multimap: ";
    print_map(othermap);
    // Pointers and references remain valid, but point to a different container after the merge
}
