#include <iterator>
#include <type_traits>
#include <utility>

/**
 * constexpr-if can be used to implement a function perfect-forwarding a callable's return type,
 * also taking into account the possibility of `void` return type (no return value).
 * decltype(auto) can not deduce `void` return type, because void is an incomplete type,
 * therefore `return callable_returnning_void()` is not valid.
 */

template <typename Callable, typename... Args>
decltype(auto) call(Callable&& c,
                    Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>)
{
    if constexpr (std::is_void_v<std::invoke_result_t<Callable, Args...>>) {
        // maybe do something before the call
        std::forward<Callable>(c)(std::forward<Args>(args)...);
        // maybe do something after the call
        return;
    }
    else {
        // maybe do something before the call
        decltype(auto) ret{std::forward<Callable>(c)(std::forward<Args>(args)...)};
        // maybe do something after the call
        return ret;
    }
}

// ---
// constexpr-if can be used to implement tag-dispatch, usually done by providing an overload set
// and helper/impl functions taking the tag argument to dispatch on, or via template
// (partial-)specialization.
// Note that with an overload set or template (partial-)specialization you get best-match semantics
// while with constexpr-if you get first-match semantics.
// Before constexpr-if
template <typename Iterator, typename Distance> void advance(Iterator& pos, Distance n)
{
    using cat = std::iterator_traits<Iterator>::iterator_category;
    advanceImpl(pos, n, cat); // tag dispatch over iterator category
}
template <typename Iterator, typename Distance>
void advanceImpl(Iterator& pos, Distance n, std::random_access_iterator_tag)
{
    pos += n;
}
template <typename Iterator, typename Distance>
void advanceImpl(Iterator& pos, Distance n, std::bidirectional_iterator_tag)
{
    if (n >= 0) {
        while (n--) {
            ++pos;
        }
    }
    else {
        while (n++) {
            --pos;
        }
    }
}
template <typename Iterator, typename Distance>
void advanceImpl(Iterator& pos, Distance n, std::input_iterator_tag)
{
    while (n--) {
        ++pos;
    }
}
// with constexpr-if
template <typename Iterator, typename Distance> void advance(Iterator& pos, Distance n)
{
    using cat = std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_same_v<cat, std::random_access_iterator_tag>) {
        pos += n;
    }
    else if constexpr (std::is_same_v<cat, std::bidirectional_access_iterator_tag>) {
        if (n >= 0) {
            while (n--) {
                ++pos;
            }
        }
        else {
            while (n++) {
                --pos;
            }
        }
    }
    else { // input_iterator_tag
        while (n--) {
            ++pos;
        }
    }
}

// constexpr-if can also be usefull when implementing a type-matching std::variant lambda visitor
auto type_matching_lambda_visitor = [](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "type-matching visitor: int value = " << arg << "\n";
    }
    else if constexpr (std::is_same_v<T, const char*>) {
        std::cout << "type-matching visitor: const char* value = " << arg << "\n";
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "type-matching visitor: std::string value = " << arg << "\n";
    }
    else {
        static_assert(always_false_v<T>, "Non-exhaustive visitor!");
    }
};

// it also supports if-with-initialization
template <typename T>
void bar(const T x)
{
    if constexpr (auto obj = foo(x); std::is_same_v<decltype(obj), T>) {
        std::cout << "foo(x) yields same type\n";
    }
    else {
        std::cout << "foo(x) yields different type\n";
    }
}

int main() {}
