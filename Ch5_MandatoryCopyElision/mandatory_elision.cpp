#include <iostream>

/**
 * C++17 makes copy elision mandatory, meaning that it is now guaranteed that no temporaries
 * are created upon copy initialization or return-by-value.
 * This also makes it possible to return-by-value even types which have both copy and move
 * constructors deleted (implicitly or explicitly).
 * All this is possible due to clarification of value categories - prvalues appearing where
 * a glvalue (lvalue or xvalue) is expected a temporary object is created in initialized with
 * the prvalue.
 */

class CopyOnly {
public:
    CopyOnly() {}
    CopyOnly(int) {}
    CopyOnly(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&) = delete; // explicitly deleted
};
CopyOnly ret()
{
    return CopyOnly{}; // OK since C++17
}
CopyOnly x = 42; // OK since C++17

int main() {}
