#pragma once

#include <chrono>
#include <string>
#include <iostream>


class Timer
{
public:
    using clock_type = std::chrono::steady_clock;

    void print_diff(char const* const msg = "Timer diff: ") {
        auto const now{clock_type::now()};
        std::chrono::duration<double, std::milli> diff{now - last_};
        std::cout << msg << diff.count() << "ms\n";
        last_ = clock_type::now();
    }
private:
    clock_type::time_point last_{clock_type::now()};
};
