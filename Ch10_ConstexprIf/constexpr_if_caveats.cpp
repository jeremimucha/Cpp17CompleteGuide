#include <string>
#include <type_traits>
#include <utility>

/**
 * constexpr-if affects the return type - depending on the branch that's taken/discarded
 * the return type will differ (`auto` return type is mandatory here)
 */
auto foo_return_type()
{
    if constexpr (sizeof(int) > 4) {
        return 42; // return int
    }
    else {
        return 42u; // return unsigned int
    }
}

// this could be even more dramatic if the alternative return type is `void`
auto foo_return_void()
{
    if constexpr (sizeof(int) > 4) {
        return 42;
    }
    // nop
}

// ---
// The `else` part always matters. Runtime-if pattern of skipping the `else` scope does not apply
// to constexpr-if:
auto foo_skip_else()
{
    if constexpr (sizeof(int) > 4) {
        return 42;
    }
    return 42u;
}
// if the condition is true the compiler deduces two return types, which is not valid.

// ---
// constexpr-if does not short-circuit evaluate - conditions in constexpr-if are always
// instantiated and need to be valid as a whole for the branch to be taken.
template <typename T>
constexpr auto bar(const T& val)
{
    if constexpr (std::is_integral<T>::value && T{} < 10) {
        return val * 2;
    }
    return val;
}
// This will result in a compiletime-error if `T` is not an integral type.

int main()
{

}
