#include <array>
#include <cstdlib>  // for std::byte
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

#include <track_new.hpp>

int main()
{
    TrackNew::reset();

    // allocate memory on the stack
    std::array<std::byte, 500000> buf;

    // use it as an initial memory pool for a vector.
    // Memory will be allocated from the given pool or from the heap as fallback - it the
    // pool memory runs out.
    std::pmr::monotonic_buffer_resource pool{buf.data(), buf.size()};

    // using std::pmr::string will let us avoid heap-allocation entirely.
    // By default std::pmr::vector (and other containers) attempt to propagate their allocator
    // to their elements. This fails for non-pmr-allocator-aware types (like std::string) in the
    // previous example, but works for pmr-allocator-aware std::pmr::string
    std::pmr::vector<std::pmr::string> coll{&pool};

    for (auto i{0}; i < 1000; ++i) {
        coll.emplace_back("some non-SSO std::string");
    }

    TrackNew::status();
}
