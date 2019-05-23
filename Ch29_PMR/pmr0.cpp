#include <iostream>
#include <string>
#include <vector>

#include <track_new.hpp>

int main()
{
    TrackNew::reset();

    std::vector<std::string> coll;
    for (auto i{0}; i < 1000; ++i) {
        coll.emplace_back("some non-SSO std::string");
    }

    TrackNew::status();
}
