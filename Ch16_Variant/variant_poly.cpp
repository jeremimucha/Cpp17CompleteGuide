#include <iostream>
#include <type_traits>
#include <vector>
#include <variant>
#include <string>
#include <utility>

/**
 * std::variant can be used as a form of compiletime-polymorphism, dealing with heterogeneous
 * collections. It doesn't require inheritance, common base classes or even a common interface.
 * Everything is based around the std::visit and visitors
 */

struct Foo {
    void print() const { std::cerr << "Foo!\n"; }
};
struct Bar {
    void print() const { std::cerr << "Bar!\n"; }
};
struct Baz {
    void print() const { std::cerr << "Baz!\n"; }
};
struct Ni {
    void draw() const { std::cerr << "NiNiNi\n"; }
};

struct Visitor {
    template<typename T>
    void operator()(T const& val) const {
        std::cerr << __PRETTY_FUNCTION__ << "\n\t";
        val.print();    // assume that most types support this common interface
    }
    // specialize for other types
    void operator()(Ni const& ni) const {
        std::cerr << __PRETTY_FUNCTION__ << "\n\t";
        ni.draw();
    }
};

template<typename... Ts>
struct overload : Ts... { using Ts::operator()...; };
template<typename... Ts>
overload(Ts...) -> overload<Ts...>;

int main()
{
    using poly_var_t = std::variant<Foo, Bar, Baz, Ni>;
    std::vector<poly_var_t> collection{Foo{}, Ni{}, Bar{}, Baz{}, Ni{}};
    for (auto const& v : collection) {
        std::visit(Visitor{}, v);
    }

    // could also use `overload`
    for (auto const& v : collection) {
        std::visit(overload{
            [](Foo const& f) { std::cerr << "over-poly: "; f.print(); },
            [](Bar const& b) { std::cerr << "over-poly: "; b.print(); },
            [](Baz const& b) { std::cerr << "over-poly: "; b.print(); },
            [](Ni const& n) { std::cerr << "over-poly: "; n.draw(); }
            },
            v
        );
    }
}
