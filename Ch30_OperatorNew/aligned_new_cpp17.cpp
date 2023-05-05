#include <iostream>
#include "aligned_new_cpp17.hpp"

int main()
{
    auto p{new MyType32};
    // auto p_aligned{new(std::align_val_t{32}) MyType32};
    delete p;
    // delete p_aligned;
}
