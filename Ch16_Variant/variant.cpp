#include <complex>
#include <iostream>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

/**
 * C++17 introduces std::variant - a tagged union type. It is implemented as a variadic template
 * type that always knows the type of the alternative it currently holds.
 */

// ---
// relavant types:
// std::variant
// std::variant_size
// std::variant_alternative
// std::monostate
// std::variant_npos
// std::bad_variant_access
// std::in_place_type_t
// std::in_place_index_t

int main()
{
    // --- construction
    {
        // - by default the default constructor of the first variant type is called, in consequence
        // this requires the first type to have a default constructor if there's a possibility that
        // it will be default constructed
        std::variant<int, int, std::string> v1;  // int at index()==0 value initialized
        // if a value is passed for initialization the best match is chosen, if two types match
        // equally the call results in abiguity, and a compiletime error
        std::variant<int, long> v2{42};  // index() == 0
        // std::variant<long, long> v3{42};    // error - ambiguous
    }
    {
        // if there's a need to disambiguate - std::in_place_index and std::in_place_type can help,
        // they can be passed as the first argument to designate which type/index should be
        // initialized, another benefit is that the designated value/type is constructed in-place
        std::variant<int, int> v1{std::in_place_index<1>, 42};  // second `int` initialized

        // std::complex constructed in place with the given values
        std::variant<std::complex<double>, double> v2{std::in_place_type<std::complex<double>>,
                                                      3.14159265359, 2.71828182846};

        // even an initializer list, and additional arguments can be passed in:
        auto const sorting_criterion = [](int x, int y) { return std::abs(x) < std::abs(y); };
        std::variant<std::vector<int>, std::set<int, decltype(sorting_criterion)>> v3{
            std::in_place_index<1>,  // which type is initialized
            {-1, 2, 4, 5, 11},       // initializer list
            sorting_criterion        // additional argument for ctor of std::set
        };

        // use std::monostate to ensure a variant is default constructible, even if all of it's
        // held alternatives are not
        struct NoDefaultCtor {
            NoDefaultCtor(int) { };
        };
        std::variant<std::monostate, NoDefaultCtor> v4{};   // default constructed, index()==0
    }

    // --- access:
    // - std::get<type|index>(variant-ref)
    // - std::get_if<type|index>(variant-ptr)
    // - visitors (see further)
    // - .index() -> index of the currently held alternative
    // - std::holds_alternative<type>()
    {
        // std::get<> with the index of the corresponding alternative, or it's type (only if there
        // are no ambiguities) can be used to access the value. Attempting to access a non-existant
        // index or type results in a compiletime error
        std::variant<int, int, std::string> var{};  // index == 0, value == 0
        // auto a = std::get<double>(var);             // compile-time ERROR: no double
        // auto b = std::get<4>(var);                  // compile-time ERROR: no 4th alternative
        // auto c = std::get<int>(var);                // compile-time ERROR: int twice
        try {
            auto s = std::get<std::string>(var);  // throws exception (first int currently set)
            auto i = std::get<0>(var);            // OK, i==0
            auto j = std::get<1>(var);            // throws exception (other int currently set)
        }
        catch (const std::bad_variant_access& e) {  // in case of an invalid access
            std::cout << "Exception: " << e.what() << '\n';
        }

        // std::get_if<> - takes a pointer to a variant, returns pointer to the held value
        // or nullptr if it currently doesn't hold the requested alternative:
        if (auto const pi = std::get_if<1>(&var); pi) {
            std::cerr << "var holds int at index 1, with value = " << *pi << "\n";
        }
        else {
            std::cerr << "alternative at index 1 not currently set\n";
        }

        // std::holds_alternative tests if the specified alternative is active - usable only
        // if it is not ambiguous
        // .index() returns the index of currently held alternative
        if (std::holds_alternative<std::string>(var)) {
            std::cerr << "var holds a std::string alternative\n";
        }
        else {
            std::cerr << "var holds an alternative at index = " << var.index() << "\n";
        }
    }

    // --- assignment
    // - operator=()
    // - std::get
    // - std::get_if
    // - emplace<type|index>
    {
        std::variant<int, int, std::string> var{};
        // assignement operator can be used if it isn't ambiguous:
        var = "hello";      // sets std::string at index()==2

        // .emplace() will set variant to the specified alternative (regardless if set or not)
        var.emplace<1>(42); // sets int at index()==1 to 42

        // std::get and get_if can be used to update the currently active alternative
        std::get<1>(var) = 11;
        // std::get<0>(var) = 22;   // throws - alternative at index()==0 not set
        if (auto pi = std::get_if<1>(&var); pi) {
            *pi = 33;
        }
    }

    // --- comparison
    // Comparison operators are supported, their behavior for two variants of the same type:
    // - A variant with a value of an earlier alternative is less than a variant with a value
    // with a later alternative.
    // - Iftwovariantshavethesamealternativethecorrespondingoperatorsforthetypeofthealternatives
    // are evaluated. Note that all objects of type std::monostate are always equal.
    // - Two variants with the special state valueless_by_exception() being true are equal. Other-
    // wise, any variant with valueless_by_exception() being true is less than any other variant.

}
