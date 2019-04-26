#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

// Using free-function get
class Bar {
public:
    template<std::size_t>
    friend struct get_impl;
private:
    int i_{11};
    double d_{2.7182182846};
    std::string str_{"Bar"};
};

template <std::size_t>
decltype(auto) get(Bar&) noexcept;

template <std::size_t>
decltype(auto) get(Bar const&) noexcept;

template <std::size_t>
decltype(auto) get(Bar&&) noexcept;

template <std::size_t>
decltype(auto) get(Bar const&&) noexcept;

template <>
struct std::tuple_size<Bar> : public std::integral_constant<size_t, 3> {  // 3 attributes
};
template <>
struct std::tuple_element<0, Bar> {
    using type = int;
};
template <>
struct std::tuple_element<1, Bar> {
    using type = double;
};
template <>
struct std::tuple_element<2, Bar> {
    using type = std::string;
};

template<std::size_t I>
struct get_impl { };

template<>
struct get_impl<0>
{
    template<typename T>
    decltype(auto) operator()(T&& t) noexcept
    {
        return std::forward<T>(t).i_;
    }
};

template<>
struct get_impl<1>
{
    template<typename T>
    decltype(auto) operator()(T&& t) noexcept
    {
        return std::forward<T>(t).d_;
    }
};

template<>
struct get_impl<2>
{
    template<typename T>
    decltype(auto) operator()(T&& t) noexcept
    {
        return std::forward<T>(t).str_;
    }
};

template<std::size_t I>
decltype(auto) get(Bar& bar) noexcept
{
    return get_impl<I>{}(bar);
}

template<std::size_t I>
decltype(auto) get(Bar const& bar) noexcept
{
    return get_impl<I>{}(bar);
}

template<std::size_t I>
decltype(auto) get(Bar&& bar) noexcept
{
    return get_impl<I>{}(std::move(bar));
}

template<std::size_t I>
decltype(auto) get(Bar const&& bar) noexcept
{
    return get_impl<I>{}(std::move(bar));
}

int main()
{
    Bar bar;
    auto [i, d, s] = bar;

    const Bar cbar;
    auto [a, b, c] = cbar;
}
