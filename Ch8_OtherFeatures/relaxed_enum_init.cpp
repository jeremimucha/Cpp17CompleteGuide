#include <iostream>

/**
 * C++17 allows using an integral value of an underlying enumeration type for direct initialization.
 * This requires underlying type to be explicitly specified for unscoped enums, and always works
 * for scoped enums (they have a default underlying type).
 * This is intended to be used for defining custom integral types. C=+17 introduces std::byte.
 */

enum class Weekday { mon, tue, wed, thu, fri, sat, sun };
Weekday s1{0}; // OK since C++17 (ERROR before C++17)
// Weekday s2 = 0; // still ERROR
// Weekday s3(0); // still ERROR
// Weekday s4 = {0}; // still ERROR

int main() {}
