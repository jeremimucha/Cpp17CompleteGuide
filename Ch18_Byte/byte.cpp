#include <bitset>
#include <cstddef>  // std::byte
#include <iostream>
#include <limits>
#include <type_traits>

/**
 * C++17 introduces `std::byte` for strongly typed operations on binary data. Its purpose is to
 * restrict integer and char conversions for binary data, while still allowing bitwise operations.
 * It's implemented as a scoped enum, with underlying type = unsigned char.
 * The relaxed scoped enum initialization rules allow for direct initialization using literals.
 * There are no implicit conversions from or to std::byte. std::to_integer template must be used
 * to cast std::byte to a desired integral type.
 * std::byte is defined in header <cstddef>
 */

// | Operation              | Effect                                                          |
// | ---------------------- | --------------------------------------------------------------- |
// | constructors           | Create a byte object (value undefined with default constructor) |
// | destructor             | Destroys a byte object (nothing to be done)                     |
// | =                      | Assign a new value                                              |
// | ==, !=, <, <=, >, >=   | Compares byte objects                                           |
// | <<, >>, |, &, ^, ~     | Binary bit-operations                                           |
// | <<=, >>=, |=, &=, ^=   | Modifying bit-operations                                        |
// | to_integer<T>()        | Converts byte object to integral type T                         |
// | sizeof()               | Yields 1                                                        |

// I/O operators must be defined manually:
std::istream& operator>>(std::istream& strm, std::byte& b)
{
    // read into a bitset:
    std::bitset<std::numeric_limits<unsigned char>::digits> bs;
    strm >> bs;
    // without failure, convert to std::byte:
    if (!std::cin.fail()) {
        b = static_cast<std::byte>(bs.to_ulong());  // OK
    }
    return strm;
}

int main()
{
    // --- construction
    // only construction using direct initialization is possible
    std::byte b{42};
    // there's no implicit conversion, which makes initializing arrays tedious
    // std::byte ba[]{1, 2};  // ERROR
    std::byte ba[]{std::byte{1}, std::byte{2}};
    // no implicit conversion to bool
    // if (b) { }  // ERROR
    if (b != std::byte{0}) {
    }  // OK
    if (std::to_integer<bool>(b)) {
    }  // OK

    // --- conversion, assigment
    // convert to an integral type using std::to_integer
    std::cerr << "b = " << std::to_integer<int>(b) << "\n";
    // can assign only another std::byte, no implicit conversions
    // b = 11; // ERROR
    b = std::byte{11};
}
