#include <complex>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>

/**
 * C++17 introduces `std::optional` - a struct holding a value of type T and a flag indicating
 * if it is holding a value or not.
 */

int main()
{
    // ---

    // Semantics of the type:
    // - construction:
    std::optional<std::string> empty_op1;  // contained type not constructed -> empty std;:optional
    std::optional<std::string> empty_op2{std::nullopt};  // same

    // - initialize with a value:
    std::optional opi{42};                         // deduces std::optional<int>
    std::optional<std::string> ops{"optional"};    // temporary std::string constructed and moved
    std::optional ops2{std::string{"optional2"}};  // same

    // - initialize by constructing in place
    std::optional<std::complex<double>> opc{std::in_place, 3.14159265359, 2.71828182846};
    auto opc2 = std::make_optional<std::complex<double>>(1.2345, 4.4567);

    // - assignment:
    opc2 = std::complex<double>{9.1234, 8.345345};  // construct and move
    opc2.emplace(4.123, 3.123);                     // construct in place
    opc2 = opc;                                     // copy assigment
    opc2.reset();           // no longer holds a value
    opc2 = {};              // no longer holds a value
    opc2 = std::nullopt;    // no longer holds a value

    // - check if it holds a value:
    std::optional<std::string> o;
    if (o) {
        std::cerr << "o = " << *o << "\n";
    }
    if (o.has_value()) {
        std::cerr << "o = " << o->c_str() << "\n";
    }
    if (!o) {
        std::cerr << "`o` doesn't hold a value\n";
    }

    // - value access:
    o.emplace("hello optional");
    // operator* - unchecked, UB if doesn't hold a value
    std::cerr << "*o = " << *o << "\n";
    // operator-> - member access, unchecked
    std::cerr << "o->c_str() = " << o->c_str << "\n";
    // .value() - checked, throws std::bad_optional_access if doesn't hold a value
    // returns by reference - care must be taken not to bind to an expired member value
    std::cerr << "o.value() = " << o.value() << "\n";
    // .value_or(some_default) - checked, returns `some_default` if doesn't hold a value
    // returns by value - might be expensive in some situations - will add cost of a move at best
    std::cerr << "o.value_or(\"foo\") = " << o.value_or("foo") << "\n";

    // - comparisons:
    // If both operands are objects with a value, the corresponding operator of the contained
    // type is used.
    // If both operands are objects without a value they are considered to be equal
    // (== yields true and all other comparisons yield false).
    // If only one operand is an object with a value the operand without a value is considered
    // to be less than the other operand.
}
