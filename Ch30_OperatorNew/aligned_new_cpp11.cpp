#include <iostream>
#include "aligned_new_cpp11.hpp"

int main()
{
    auto p{new MyType32{}};
    delete p;
}
