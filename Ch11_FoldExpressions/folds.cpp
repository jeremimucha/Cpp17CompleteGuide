#include <iostream>
#include <type_traits>

/**
 * C++17 introduced `fold expressions`:
 * - left fold:
 *   (... `op` args)
 *   expands to: ((arg1 op arg2) op arg3) op ...
 * - right fold:
 *   (... `op` args)
 *   expands to: arg1 op (arg2 op ... (argN-1 op argN))
 *
 * Theres also a form which takes an initial value/argument:
 * - binary left fold:
 *   (init `op` ... `op` args)
 *   expands to: ((init op arg1) op arg2) op ...
 * - binary right fold:
 *   expands to: arg1 op (arg2 op ... (argN op init))
 *
 * If operator && is used, the value is true.
 * If operator || is used, the value is false.
 * If the comma operator is used, the value is void().
 * For all other operators the call is ill-formed.
 */

// example implementation of print:
template <typename... T>
void print1(const T&... args)
{
    (std::cout << ... << args) << '\n';
}

// This could be refined, by adding a separateor in between the printed arguments:
template <typename T>
decltype(auto) space_before(T const& arg)
{
    std::cout << " ";
    return arg;
}

template <typename T, typename... Ts>
void print2(T const& first, Ts const&... args)
{
    std::cout << first;
    (std::cout << ... << space_before(args)) << "\n";
}

// could also be done with a lambda. Note that lambdas return by-value by default, so we need to
// explicitly declare the reutrn type
template <typename T, typename... Ts>
void print3(T const& first, Ts const&... args)
{
    auto const print_space = [](auto const& arg) -> decltype(auto) {
        std::cout << " ";
        return arg;
    };
    std::cout << first;
    (std::cout << ... << print_space(args)) << "\n";
}

// It would probably be cleaner to eliminate the intermediate return completely and just print
// the argument with the preceeding space. Everything could be done in a single expression:
template <typename T, typename... Ts>
void print4(T const& first, Ts const&... args)
{
    std::cout << first;
    (..., [](auto const& arg) { std::cout << " " << arg; }(args));
    std::cout << "\n";
}

// A further improvement would be to parameterize the separator:
template <auto Sep = ' ', typename T, typename... Ts>
void print5(T const& first, Ts const&... args)
{
    std::cout << first;
    auto const print_with_sep = [](auto const& arg) { std::cout << Sep << arg; };
    (..., print_with_sep(args));
    std::cout << "\n";
}

// Fold expressions can also be used to implement type-traits operating on template parameter packs:
template<typename T, typename... Ts>
struct IsHomogenous {
    static constexpr bool value{... && std::is_same_v<T, Ts>};
};

template<typename T, typename... Ts>
constexpr inline bool IsHomogenousV{IsHomogenous<T, Ts...>::value};

template<typename T, typename... Ts>
constexpr bool isHomogenous(T, Ts...)
{
    return (... && std::is_same_v<T, Ts>);
}

int main() {}
