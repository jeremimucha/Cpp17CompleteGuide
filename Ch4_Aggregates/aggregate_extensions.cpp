#include <iostream>

/**
 * C++17 extends aggregates to also include structures deriving from other classes/structs.
 * The other rules still apply, aggregates are either arrays or structs which
 * – no user-declared or explicit constructor
 * – no constructor inherited by a using declaration
 * – no private or protected non-static data members
 * – no virtual functions
 * – no virtual, private, or protected base classes
 */

struct Data {
    const char* name;
    double value;
};
// Aggregate since C++17
struct PData : Data {
    bool critical;
    void print() const { std::cout << '[' << name << ',' << value << "]\n"; }
};

// This is fine even if the base class is not an aggregate:
struct MyString : std::string {
    void print() const
    {
        if (empty()) {
            std::cout << "<undefined>\n";
        }
        else {
            std::cout << c_str() << '\n';
        }
    }
};

// This actually breaks some code - where the derived class would expect the default
// constructor of the base class to be available:
struct Derived;
struct Base {
    friend struct Derived;
private:
    Base() {}
};
struct Derived : Base {
};

int main()
{
    PData y{{"test1", 6.778}, false};
    y.print();
    PData a{};         // zero-initialize all elements
    PData b{{"msg"}};  // same as {{"msg",0.0},false}
    PData c{{}, true}; // same as {{nullptr,0.0},true}
    PData d;           // values of fundamental types are unspecified

    // Derived dd{};    // ERROR since C++17 - Derived is an aggregate,
                        // aggregate initialization can not call a private constructor of Base
                        // pre C++17 Derived was not an aggregate, the implicitly declared
                        // ctor would call the private, but accessible via friend, ctor of Base
}
