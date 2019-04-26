#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/type_index.hpp>

/**
 * Structured bindings can be used for:
 * - arrays
 * - any struct or class with all-public member data
 * - std::tuple
 * - std::pair
 * - std::array
 * - any user-defined type providing a tuple-like api - specializations of:
 *   - template<> struct std::tuple_size<user_type> { static constexpr auto value = ...; };
 *   - template<> struct std::tuple_element<0, user_type> { using type = ...; };
 *   - template<std::size_t I> decltype(auto) get(user_type&);
 *
 * Type qualifiers used with structured bindings don't directly affect the bindings themselves
 * but rather an implicitly declared anonymous entity to members of which the bindings refer
 */

template <typename... T>
void print_types(std::string_view info)
{
    using boost::typeindex::type_id_with_cvr;
    const auto add_sep = [](auto&& s) { std::cout << s << ", "; };
    std::cout << info;
    (..., add_sep(type_id_with_cvr<T>().pretty_name()));
    std::cout << "\n";
}

void array_bindings()
{
    std::cout << "case for const int arr[2]:\n";

    int arr[2] = {1, 2};

    auto [a0, a1] = arr;  // copies arr, a0, a1 refer to the elements of the copy
    static_assert(std::is_same_v<decltype(a1), int>, "");
    print_types<decltype(a0)>("auto [a0,a1]:               ");
    const auto [ca0, ca1] = arr;
    print_types<decltype(ca0)>("const auto [ca0,ca1]:       ");
    static_assert(std::is_same_v<decltype(ca0), const int>, "");
    auto& [a_r0, a_r1] = arr;  // refers to the elements of arr
    print_types<decltype(a_r0)>("auto& [a_r0,a_r1]:          ");
    static_assert(std::is_same_v<decltype(a_r0), int>, "");
    auto const& [a_cr0, a_cr1] = arr;
    print_types<decltype(a_cr0)>("const auto& [a_cr0,a_cr1]:  ");
    static_assert(std::is_same_v<decltype(a_cr0), const int>, "");
    auto&& [a_rr0, a_rr1] = arr;
    print_types<decltype(a_rr0)>("auto&& [a_rr0,a_rr1]:       ");
    static_assert(std::is_same_v<decltype(a_rr0), int>, "");

    // All of the reported types are that of the original member data, possibly with additional
    // qualifiers. However the structured binding are references/aliases to the member data,
    // and not copies. Changing the bindings changes the corresponding member data field.
    // The above works the same for std::pair, std::tuple and any user-defined type with
    // all public member data.
}

int main()
{
    array_bindings();
    std::cerr << "\n\n";
    std::pair<int, double> p{9, 3.14159265359};
    auto [a, b] = p;    // implicit copy of p
    std::cerr << "&p = " << &p << "\n";
    std::cerr << "&a = " << &a << "\n";
    std::cerr << "&p != &a: " << std::boolalpha << (&p != reinterpret_cast<std::pair<int,double>*>(&a)) << "\n";
    std::cerr << "[a, b] = [" << a << ", " << b << "]\n";

    // once declared, bindings can later be rebound to other member data
    std::tuple<int, double> t{42, 2.718281828};
    std::tie(a, b) = t;
    std::cerr << "after std::tie = [" << a << ", " << b << "]\n";

    // used with a reference-binding will assign to the referenced to object:
    std::cerr << "\nauto& [c, d]:\n";
    auto& [c, d] = p;
    std::cerr << "[c, d] = [" << c << ", " << d << "]\n";
    std::tie(c, d) = t;
    std::cerr << "[c, d] after std::tie = [" << c << ", " << d << "]\n";
    std::cerr << "p after std::tie on [c, d] = " << p.first << ", " << p.second << "\n";
}
