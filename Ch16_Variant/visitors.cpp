#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

template <typename... Ts>
struct always_false : std::false_type {
};
template <typename... Ts>
using always_false_t = typename always_false<Ts...>::type;
template <typename... Ts>
constexpr inline auto always_false_v{always_false<Ts...>::value};

/**
 * The more convenient and readable way to access std::variant's active alternative
 * without manually querrying the index is to utilise std::visit with appropriately defined
 * visitors:
 */
class MyVisitor {
public:
    constexpr explicit MyVisitor(std::ostream& os) : os_{os} {}

    void operator()(int i) noexcept { os_ << "MyVisitor: int value = " << i << "\n"; }

    void operator()(double d) noexcept { os_ << "MyVisitor: double value = " << d << "\n"; }

    void operator()(const std::string& str) noexcept
    {
        os_ << "MyVisitor: std::string value = " << str << "\n";
    }

private:
    std::ostream& os_;
};

// --- Visitors can also be used to modify the value of the current alternative,
// but not to assign a new alternative:
struct Twice {
    constexpr void operator()(int& i) const noexcept { i *= 2; }
    constexpr void operator()(double& d) const noexcept { d *= 2; }
    void operator()(std::string& str) const { str += str; }
};

// Variadic overload type for inheriting from lambdas...
template <typename... Ts>
struct overload : public Ts... {
    using Ts::operator()...;
};
// template deduction guide
template <typename... Ts>
overload(Ts...)->overload<Ts...>;

// Go a step further - provide a nicer interface
template <typename Variant, typename... Ts>
decltype(auto) match(Variant&& v, Ts&&... ts)
{
    return std::visit(overload{std::forward<Ts>(ts)...}, std::forward<Variant>(v));
}

int main()
{
    std::variant<int, std::string, double> var{42};
    // - calls MyVisitor::operator()(int)
    std::visit(MyVisitor{std::cerr}, var);
    var.emplace<std::string>("Hello!");
    // - calls MyVisitor::operator()(std::string)
    std::visit(MyVisitor{std::cerr}, var);
    var = 3.14159265359;
    std::visit(Twice{}, var);

    // The easiest way to use visitors is to utilise generic lambdas:
    std::visit([](auto const& val) { std::cerr << "value = " << val << "\n"; }, var);

    // use a generic lambda, but also utilise `if constexpr` to, at compile time, identify the type
    // of the variable held by the variant. This is an alternative to a class with overloaded
    // operator()'s
    std::cerr << "\nType matching lambda visitor...\n";
    auto const type_matching_lambda_visitor = [](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cerr << "type-matching visitor: int value = " << arg << "\n";
        }
        else if constexpr (std::is_same_v<T, double>) {
            std::cerr << "type-matching visitor: double value = " << arg << "\n";
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::cerr << "type-matching visitor: std::string value = " << arg << "\n";
        }
        else {
            static_assert(always_false_v<T>, "Non-exhaustive visitor!");
        }
    };
    std::visit(type_matching_lambda_visitor, var);

    // std::visit can also be used with multiple std::variant instances,
    // the instances don't even need to be of the same type - as long as the defined visitor
    // is exhaustive.
    // For multiple `variant` instances to overloading/matching is done in pairs:
    std::variant<char const*, bool> var2{std::in_place_type<bool>, true};
    auto const multivariant_visitor = [](auto&& arg1, auto&& arg2) {
        using T1 = std::decay_t<decltype(arg1)>;
        using T2 = std::decay_t<decltype(arg2)>;
        if constexpr (std::is_same_v<T1, int>) {
            if constexpr (std::is_same_v<T2, char const*>) {
                std::cerr << "multivariant {int, char const*} = {" << arg1 << ", " << arg2 << "}\n";
            }
            else if constexpr (std::is_same_v<T2, bool>) {
                std::cerr << "multivariant {int, bool} = {"
                    << arg1 << ", " << std::boolalpha << arg2 << "}\n";
            }
            else {
                static_assert(always_false_v<T1, T2>, "non-exhaustive visitor!");
            }
        }
        else if constexpr(std::is_same_v<T1, std::string>) {
            if constexpr (std::is_same_v<T2, char const*>) {
                std::cerr << "multivariant {std::string, char const*} = {"
                    << arg1 << ", " << arg2 << "}\n";
            }
            else if constexpr (std::is_same_v<T2, bool>) {
                std::cerr << "multivariant {std::string, bool} = {"
                    << arg1 << ", " << std::boolalpha << arg2 << "}\n";
            }
            else {
                static_assert(always_false_v<T1, T2>, "non-exhaustive visitor!");
            }
        }
        else if constexpr (std::is_same_v<T1, double>) {
            if constexpr (std::is_same_v<T2, char const*>) {
                std::cerr << "multivariant {double, char const*} = {"
                    << arg1 << ", " << arg2 << "}\n";
            }
            else if constexpr (std::is_same_v<T2, bool>) {
                std::cerr << "multivariant {double, bool} = {"
                    << arg1 << ", " << std::boolalpha << arg2 << "}\n";
            }
            else {
                static_assert(always_false_v<T1, T2>, "non-exhaustive visitor!");
            }
        }
        else {
            static_assert(always_false_v<T1>, "non-exhaustive visitor!");
        }
    };
    std::visit(multivariant_visitor, var, var2);

    // use the `overload` template to provide lambdas for each alternative in place
    var.emplace<std::string>("Hello overload!");
    std::visit(overload{
        [](int const i) noexcept {std::cerr << "overload: int = " << i << "\n"; },
        [](double const d) noexcept { std::cerr << "overload: double = " << d << "\n"; },
        [](std::string const& str) noexcept {std::cerr << "overload: string = " << str << "\n"; }
        },
        var);

    // use `overload` to define a multivariant visitor (somewhat more readable?):
    std::visit(overload{
            [](int i, char const* str) {
                std::cerr << "multi-overload {int, char const*} = {" << i << ", " << str << "}\n";
            },
            [](int i, bool b){
                std::cerr << "multi-overload {int, bool} = {"
                    << i << ", " << std::boolalpha << b << "}\n";
            },
            [](std::string const& s1, char const* s2) {
                std::cerr << "multi-overload {std::string, char const*} = {"
                    << s1 << ", " << s2 << "}\n";
            },
            [](std::string const& s, bool b) {
                std::cerr << "multi-overload {std::string, bool} = {"
                    << s << ", " << std::boolalpha << b << "}\n";
            },
            [](double d, char const* s) {
                std::cerr << "multi-overload {double, char const*} = {"
                    << d << ", " << s << "}\n";
            },
            [](double d, bool b) {
                std::cerr << "multi-overload {double, bool} = {"
                    << d << ", " << std::boolalpha << b << "}\n";
            }
        },
        var,
        var2
    );

    // or use, a potentially more readable convenience `match` - only usable for single variant
    std::get<std::string>(var) = "Hello match!";
    match(var,
        [](int const i) noexcept {std::cerr << "overload: int = " << i << "\n"; },
        [](double const d) noexcept { std::cerr << "overload: double = " << d << "\n"; },
        [](std::string const& str) noexcept {std::cerr << "overload: string = " << str << "\n"; }
    );
}
