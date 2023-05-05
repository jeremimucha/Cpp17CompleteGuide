#include <iostream>
#include <string>

/**
 * Since C++17 the expression evaluation order is explicitly specified.
 * The evaluation order is now guaranteed to be from left to right for the following operators:
 *   e1 [ e2 ]
 *   e1 . e2
 *   e1 . * e2
 *   e1 -> * e2
 *   e1 << e2
 *   e1 >> e2
 *
 * For assignment operators the right hand side is evaluated first:
 *   e2 = e1
 *   e2 += e1
 *   e2 * = e1
 *
 * For `new` expressions:
 *   new T{e};
 * allocation is guaranteed to be performed before evaluating `e`, and initialization is guaranteed
 * to happen before any use if the allocated and initialized value.
 * 
 * Note that this is potentially a breaking change.
 */

int main()
{
    // guaranteed consistent result since C++17:
    std::string s{"I heard it even works if you don't believe"};
    std::cerr << s << "\n";
    s.replace(0, 8, "")
        .replace(s.find("even"), 4, "always")
        .replace(s.find("don't believe"), 13, "use C++17");
    std::cerr << s << "\n";
}
