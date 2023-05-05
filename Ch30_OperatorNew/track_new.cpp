#include "track_new.hpp"
#include <iostream>
#include <string>


int main()
{
    TrackNew::reset();
    TrackNew::trace(true);
    std::string s{"string value with 26 chars"};
    auto p1 = new std::string{"an initial value with even 35 chars"};
    // MSVC complains here - error: sized deallocation function 'operator delete(void*, size_t)'
    // would be chosen as placement deallocation function.
    // auto p2 = new(std::align_val_t{64}) std::string[4];
    auto p3 = new std::string[4] {"7 chars", "x", "or 11 chars", "a string value with 28 chars"};
    TrackNew::status();
    delete p1;
    // delete[] p2;
    delete[] p3;

    // output on MSVC 19.20 (with the aligned alloc commented out):
    // #1 ::new (8 bytes, def-aligned) => 0146B018 (total: 8 Bytes)
    // #2 ::new (32 bytes, def-aligned) => 01469F00 (total: 40 Bytes)
    // #3 ::new (28 bytes, def-aligned) => 01468038 (total: 68 Bytes)
    // #4 ::new (8 bytes, def-aligned) => 0146B088 (total: 76 Bytes)
    // #5 ::new (48 bytes, def-aligned) => 0146CE00 (total: 124 Bytes)
    // #6 ::new[] (116 bytes, def-aligned) => 0146E410 (total: 240 Bytes)
    // #7 ::new (8 bytes, def-aligned) => 0146B130 (total: 248 Bytes)
    // #8 ::new (8 bytes, def-aligned) => 0146AFE0 (total: 256 Bytes)
    // #9 ::new (8 bytes, def-aligned) => 0146B360 (total: 264 Bytes)
    // #10 ::new (8 bytes, def-aligned) => 0146B398 (total: 272 Bytes)
    // #11 ::new (32 bytes, def-aligned) => 014699B0 (total: 304 Bytes)
    // 11 allocations for 304 bytes
}
