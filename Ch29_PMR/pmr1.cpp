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

    // deduces std::pmr::vector<std::string, std::pmr::polymorphic_allocator<std::string>>
    std::pmr::vector<std::string> coll{&pool};

    // This will still allocate memory on the heap!
    // The vector will acquire memory from the pool, but string still allocates using
    // the standard allocator.
    for (auto i{0}; i < 1000; ++i) {
        coll.emplace_back("some non-SSO std::string");
    }

    TrackNew::status();
}
