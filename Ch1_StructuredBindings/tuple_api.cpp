#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

// Using free-function get
class Bar {
public:
    template <std::size_t> friend decltype(auto) get(Bar&) noexcept;

    template <std::size_t> friend decltype(auto) get(Bar const&) noexcept;

    template <std::size_t> friend decltype(auto) get(Bar&&) noexcept;

    template <std::size_t> friend decltype(auto) get(Bar const&&) noexcept;

    // private:
    int i_{11};
    double d_{2.7182182846};
    std::string str_{"Bar"};
};
template <> struct std::tuple_size<Bar> : public std::integral_constant<size_t, 3> { // 3 attributes
};
template <> struct std::tuple_element<0, Bar> {
    using type = int;
};
template <> struct std::tuple_element<1, Bar> {
    using type = double;
};
template <> struct std::tuple_element<2, Bar> {
    using type = std::string;
};

// namespace scope declaration is necessary for proper name resolution
template <std::size_t> decltype(auto) get(Bar&) noexcept;
template <std::size_t> decltype(auto) get(Bar const&) noexcept;
template <std::size_t> decltype(auto) get(Bar&&) noexcept;
template <std::size_t> decltype(auto) get(Bar const&&) noexcept;

template <> decltype(auto) get<0>(Bar& b) noexcept { return b.i_; }
template <> decltype(auto) get<1>(Bar& b) noexcept { return b.d_; }
template <> decltype(auto) get<2>(Bar& b) noexcept { return b.str_; }

template <> decltype(auto) get<0>(Bar const& b) noexcept { return b.i_; }
template <> decltype(auto) get<1>(Bar const& b) noexcept { return b.d_; }
template <> decltype(auto) get<2>(Bar const& b) noexcept { return b.str_; }

template <> decltype(auto) get<0>(Bar&& b) noexcept { return std::move(b.i_); }
template <> decltype(auto) get<1>(Bar&& b) noexcept { return std::move(b.d_); }
template <> decltype(auto) get<2>(Bar&& b) noexcept { return std::move(b.str_); }

template <> decltype(auto) get<0>(Bar const&& b) noexcept { return std::move(b.i_); }
template <> decltype(auto) get<1>(Bar const&& b) noexcept { return std::move(b.d_); }
template <> decltype(auto) get<2>(Bar const&& b) noexcept { return std::move(b.str_); }

int main()
{
    Bar bar;
    auto [i, d, s] = bar;

    const Bar cbar;
    auto [a, b, c] = cbar;
}
