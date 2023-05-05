#ifndef TO_FROM_CHARS_HEADER_GUARD
#define TO_FROM_CHARS_HEADER_GUARD

#include <charconv>     // for std::to_chars, std::from_chars
#include <iostream>
#include <cassert>

void d2str2d(double value)
{
    std::cerr << "input: " << value << "\n";

    // convert to character sequence,
    // a large char array is required to store the full representation of the given `double`,
    // storing the full representation is the only guarantee that reversing the conversion
    // back to `double` will give the exact same value.
    char str[1000];
    // `std::to_chars` converts a numeric `value` to a char-string representation, it returns
    // `std::to_chars_result` which contains a `ptr` to one-past the last written character,
    // and a `std::errc` ec, which is equal to std::errc{} if the conversion was successful
    std::to_chars_result to_res = std::to_chars(str, str+999, value);
    *to_res.ptr = '\0'; // have to add the terminating null manually
    std::cerr << "str: " << str << "\n";
    // no implicit conversions for std::errc - need to compare explicitly against std::errc{}
    assert(to_res.ec == std::errc{});

    // `std::from_chars` converts a character-sequence to a numeric value, it returns a
    // `std::from_chars_result` which holds a `ptr` to the first char not parsed as part of the
    // number, and a `std::errc` ec, which indicates an error, ec == std::errc{} in case of
    // successful conversion.
    double res_value;
    std::from_chars_result from_res = std::from_chars(str, str+999, res_value);
    assert(from_res.ec == std::errc{});

    // On the same architecture/platform the conversion to and back from chars should result
    // in an exactly equal value:
    assert(value == res_value);
}

#endif // TO_FROM_CHARS_HEADER_GUARD
