#include <any>
#include <complex>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

/**
 * C++17 adds `std::any` - a value type that's able to change its type, but while still having
 * type safety. std::any accepts an arbitrary type - it holds the contained value and the
 * `typeid` of its type - thus there's both a size and a runtime cost, including the possibility
 * that std::any might allocate - the implementation should be able to hold small type like int
 * without allocating, but larger type (larger than a pointer, two pointers?) will be allocated
 * on the heap.
 * The underlying type can be checked by comparing against typeid(T),
 * value can be accessed via std::any_cast<T>
 */

/*
 | Operation     | Effect                                                            |
 | ------------- | ----------------------------------------------------------------- |
 | constructors  | Create an any object (might call constructor for underlying type) |
 | make_any()    | Create an any object (passing value(s) to initialize it)          |
 | destructor    | Destroys an any object                                            |
 | =             | Assign a new value                                                |
 | emplace<T>()  | Assign a new value having the type T                              |
 | reset()       | Destroys any value (makes the object empty)                       |
 | has_value()   | Returns whether the object has a value                            |
 | type()        | Returns the current type as std::type_info object                 |
 | any_cast<T>() | Use current value as value of type T (exception if other type)    |
 | swap()        | Swaps values between two objects                                  |
*/

int main()
{
    // --- construction
    // - initialized empty by default, .type() of an empty std::any is equal to typeid(void)
    {
        std::any a{};  // empty
        if (a.type() == typeid(void)) {
            std::cerr << "std::any{}.type() == typeid(void)\n";
        }

        // deduce the type by direct initialization or assignment, deduced type decays
        std::any a1{42};
        if (a1.type() == typeid(int)) {
            std::cerr << "std::any{42}.type() == typeid(int)\n";
        }
        std::any a2 = "hello any!";
        if (a2.type() == typeid(char const*)) {
            std::cerr << "std::any{\"hello any!\"} == typeid(char const*)\n";
        }

        // std::in_place_type can be used for construction, it allows to specify the type
        // directly, and avoids copy/move
        std::any a3{std::in_place_type<std::string>, "hello any!"};
        // note that even the type passed to std::in_place_type decays!
        std::any a4{std::in_place_type<char const[6]>, "Hello"};
        if (a4.type() == typeid(char const*)) {
            std::cerr << "std::any{std::in_place_type<char const[6]>{'hello'} == typeid(char "
                         "const*) -> decays!\n";
        }

        // std::make_any - always have to specify the type, also decays
        auto a5 = std::make_any<long>(42);
        auto a6 = std::make_any<std::string>("hello make_any!");
    }
    // --- assignment
    // values can be assigned using operator=() or .emplace<T>
    {
        std::any a{};
        a = 42;  // .type() == typeid(int)
        // .emplace()
        a.emplace<std::string>("hello emplace");
        if (a.type() == typeid(std::string)) {
            std::cerr << "a.emplace<std::string> -> value == "
                      << std::any_cast<std::string const&>(a) << "\n";
        }
        // .reset() - makes the std::any empty
        a.reset();
        if (a.type() == typeid(void)) {
            std::cerr << "std::any empty after .reset() -> .type() == typeid(void)\n";
        }
    }
    // --- value access
    // check if holds a value using .has_value()
    // check type of value using .type() -> std::type_info -> compare against typeid(T)
    // access using std::any_cast<T> -> throws std::bad_any_cast if doesn't hold a value
    // of the requested type. Can cast to references or pointers
    {
        std::any a{42};
        // casting to an unqualified type, returns by value - copy
        auto i = std::any_cast<int>(a);
        std::cerr << "std::any holds an int value = " << i << "\n";
        a.emplace<std::string>("Hello any_cast");
        // we might want to avoid a copy by casting to a const&:
        auto const& scr = std::any_cast<std::string const&>(a);
        std::cerr << "std::any_cast<std::string const&>(a) = " << scr << "\n";
        // casting to a non-const allows us to assign to the value held:
        std::any_cast<std::string&>(a) = "Update using any_cast";
        std::cerr << "std::any after update, value = " << std::any_cast<std::string const&>(a)
                  << "\n";
        // We can also move assign the same way
        std::string s{"move assign using any_cast"};
        std::any_cast<std::string&>(a) = std::move(s);

        // using std::any_cast on a pointer to std::any returns a pointer to the held value,
        // if the actual type matches the requested type, or nullptr if it doesn't
        // this might be a convenient option instead of explicitly checking .type()
        // Cast to type `T` when using this from,
        // attempting to cast to `T*` will result in the implementation assuming that you're
        // expecting the held type to be `T*` (and not `T`)
        // attempting to cast to `T&` is an error
        auto pi = std::any_cast<int>(&a);   // returns nullptr, would throw if used on ref to any
        auto ps = std::any_cast<std::string const>(&a);   // returns pointer to std::string
        if (pi != nullptr ){
            std::cerr << "std::any_cast<int>(&a) returned a valid pointer, value = " << *pi << "\n";
        }
        else {
            std::cerr << "std::any_cast<int>(&a) returned a nullptr\n";
        }
        if (ps != nullptr) {
            std::cerr << "std::any_cast<std::string>(&a) returned a valid pointer, value = "
                << *ps << "\n";
        }
        else {
            std::cerr << "std::any_cast<std::string>(&a) returned a nullptr\n";
        }
    }
}
