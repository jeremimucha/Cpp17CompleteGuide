#include <iostream>
#include <string>
#include <type_traits>

/**
 * C++17 introduced `if constexpr` expression - it is used to choose between the `then` and `else`
 * parts of the expression at compile time. The part not chosen is discarded - so that
 * no code is generated. It's an alternative to template (partial-)specialization, that can
 * be more readable in most cases.
 */

// In the following example some conversion to string is performed, depending on the type of `T`
// passed to the function. The example would not compile at all with runtime-if, since in all
// cases at least on of the branches would result in a compiletime error.
template <typename T> std::string asString(T x)
{
    if constexpr (std::is_same_v<T, std::string>) {
        return x; // statement invalid, if no conversion to string
    }
    else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(x); // statement invalid, if x is not numeric
    }
    else {
        return std::string(x); // statement invalid, if no conversion to string
    }
}

// The discarded part of the expression is not ignored entirely. It still must be syntactically
// correct. The translation (definition) phase is performed, the discarded part is just not
// instantiated. Also - all static_asserts must pass, regardless if the part containing
// the static_assert is discarded or not.
template <typename T>
void foo(T t)
{
    if constexpr (std::is_integral_v<T>) {
        if (t > 0) {
            foo(t - 1); // OK
        }
    }
    else {
        undeclared(t); // error if not declared and not discarded (i.e., T is not integral)
        undeclared();  // error if not declared (even if discarded)
        static_assert(false, "no integral"); // always asserts (even if discarded)
    }
}

int main()
{

}
