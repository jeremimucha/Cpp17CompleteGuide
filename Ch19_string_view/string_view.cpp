#include <charconv>
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

/**
 * C++17 introduces std::string_view - a 'view' into a string - meaning that it's non-mutable,
 * string_view can not be used to write to the pointed-to data.
 * std::string_view points to a sequence of characters - a c-string, std::string, memory mapped file
 * etc. It creates dangers similar to a raw pointer - it does not extend the lifetime of the data,
 * and can potentially hold a nullptr (e.g. when default-initialized).
 */
// | Operation                       | Effect                                             |
// | ------------------------------- | -------------------------------------------------- |
// | constructors                    | Create or copy a string view                       |
// | destructor                      | Destroys a string view                             |
// | =                               | Assign a new value                                 |
// | swap()                          | Swaps values between two strings view              |
// | ==, !=, <, <=, >, >=, compare() | Compare string views                               |
// | empty()                         | Returns whether the string view is empty           |
// | size(), length()                | Return the number of characters                    |
// | max_size()                      | Returnsthemaximumpossiblenumberofcharacters        |
// | [], at()                        | Access a character                                 |
// | front() back()                  | Access the first or last character                 |
// | <<                              | Writes the value to a stream                       |
// | copy()                          | Copies or writes the contents to a character array |
// | data()                          | Returns the value as nullptr or constant character |
//                                     array (note: no terminating null character)        |
// | find functions                  | Search for a certain substring or character        |
// | begin(), end()                  | Provide normal iterator support                    |
// | cbegin(), cend()                | Provide constant iterator support                  |
// | rbegin(), rend()                | Provide reverse iterator support                   |
// | crbegin(), crend()              | Provide constant reverse iterator support          |
// | substr()                        | Returns a certain substring                        |
// | remove_prefix()                 | Remove leading characters                          |
// | remove_suffix()                 | Remove trailing characters                         |
// | hash<>                          | Function object type to compute hash values        |

// example usage of std::string_view
std::optional<int> asInt(std::string_view sv)
{
    int val;
    // read character sequence into the int:
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
    // if we have an error code, return no value:
    if (ec != std::errc{}) {
        return std::nullopt;
    }
    return val;
}

std::string toString(std::string_view prefix, const std::chrono::system_clock::time_point& tp)
{
    auto rawtime = std::chrono::system_clock::to_time_t(tp);
    std::string_view ts = std::ctime(&rawtime);  // NOTE: not thread safe
    ts.remove_suffix(1);                         // skip trailing newline
    // std::string(prefix) + ts;             // unfortunately no operator + yet
    return std::string{prefix} + std::string{ts};
}

// Rules for using std::string_view:
// * Don’t use string views in APIs that pass the argument to a string.
//   – Don’t initialize string members from string view parameters.
//   – No string at the end of a string view chain.
// * Don’t return a string view.
//   – Unless it is just a forwarded input argument or you signal the danger by, for example, naming
//   the function accordingly.
// * For this reason, function templates should never return the type T of a passed generic
// argument.
//   – Return auto instead.
// * Never use a returned value to initialize a string view.
// * For this reason, don’t assign the return value of a function template returning a generic
//   type to auto.
//   – This means, the AAA (Almost Always Auto) pattern is broken with string view.

// In general - treat std::string_view as you would a raw, non-owning pointer,
// use it in call-chains that respect the lifetime of the view - i.e. don't expect it to be valid
// longer than the call.

int main()
{
    // --- construction
    // constructed from std::string, raw-string or literal suffix `sv`
    {
        std::string_view sv{};          // empty - nullptr data - access is UB
        std::string_view svc{"hello"};  // .size() == 5 ('\0' not counted)
        // attempting to access one-past-end (i.e. the '\0' char is UB)
        // svc.at(5);  // throws std::out_of_range exception
        // svc[5]; // UB
        // can create a string_view that explicitly includes the null-terminator
        std::string_view sv_null{"hello", 6};  // .size() == 6 - includes '\0'

        // construct from literal suffix
        using namespace std::literals;
        auto svl{"hello"sv};
    }

    // --- mutating
    {
        // string_view can be assigned to and swapped,
        std::string_view sv1{"hello"};
        std::string_view sv2{"world"};
        std::swap(sv1, sv2);
        sv2 = sv1;

        // we can drop the characters at the front or end of string_view - this just moves the
        // begin pointer or changes the length of the range string_view refers to
        std::string_view sv{", some text! I"};
        sv.remove_prefix(2);  // drop first two chars
        sv.remove_suffix(3);  // drop last three chars
    }
}
