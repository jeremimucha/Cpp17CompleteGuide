#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

class Foo {
public:
    // Member get() should not be used - preffer free function defined in the same namespace as
    // the type for which it is implemented.
    // Note that a free-function needs to either use some public API of the class
    // or be declared a friend. If neither is possible then member get() might be
    // worth consideration
    template <std::size_t N> constexpr std::tuple_element_t<N, Foo> get() const&
    {
        return get_impl<N>(*this);
    }

    template <std::size_t N> constexpr std::tuple_element_t<N, Foo> get() &
    {
        return get_impl<N>(*this);
    }

    template <std::size_t N> constexpr std::tuple_element_t<N, Foo> get() &&
    {
        return get_impl<N>(std::move(*this));
    }

    template <std::size_t N> constexpr std::tuple_element_t<N, Foo> get() const&&
    {
        return get_impl<N>(std::move(*this));
    }

private:
    // One possible implementation. Another approach would be to define a full specializataion
    // for every N
    template <std::size_t N, typename T>
    static constexpr auto get_impl(T&& self) -> decltype(self.template get<N>())
    {
        static_assert(0 <= N && N < 3, "Foo has only 3 elements");
        if constexpr (N == 0) {
            return std::forward<T>(self).i_;
        }
        else if constexpr (N == 1) {
            return std::forward<T>(self).d_;
        }
        else if constexpr (N == 2) {
            return std::forward<T>(self).str_;
        }
    }
    int i_{42};
    double d_{3.14159265359};
    std::string str_{"Foo"};
};

template <> struct std::tuple_size<Foo> : public std::integral_constant<size_t, 3> { // 3 attributes
};
template <> struct std::tuple_element<0, Foo> {
    using type = int;
};
template <> struct std::tuple_element<1, Foo> {
    using type = double;
};
template <> struct std::tuple_element<2, Foo> {
    using type = std::string;
};

int main()
{
    Foo f;

    auto [i, d, s] = f;

    const Foo cf;
    auto [a, b, c] = cf;
}
