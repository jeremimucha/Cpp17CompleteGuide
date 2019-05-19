#include <iostream>
#include <string>
#include <vector>
#include <utility>

/**
 * C++17 requires std::vector, std::list and std::forward_list to support incomplete types.
 */

class Node {
public:
    Node(std::string s)
        : value_{std::move(s)} { }

    void add(Node n) { children_.push_back(std::move(n)); }
    Node& operator[](std::size_t idx) { return children_.at(idx); }
    // print recursively
    void print(int indent = 0) const
    {
        for (auto i{0}; i != indent; ++i) { std::cout << " "; }
        std::cout << value_ << "\n";
        for (const auto& ch : children_) { ch.print(indent+2); }
    }

private:
    std::string value_;
    std::vector<Node> children_{};
};

int main()
{
    Node root{"top"};
    root.add(Node{"elem 1"});
    root.add(Node{"elem2"});
    root[0].add(Node{"elem 1.1"});
    root[1].add(Node{"elem 2.1"});
    root.print();
}
