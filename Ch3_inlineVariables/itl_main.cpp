#include <thread>
#include <future>
#include "inline_thread_local.hpp"
#include "use_itl.hpp"

/**
 * Note - doesn't compile on g++ 8.3 or clang 8.0 on MSYS2, works on MSVC
 */

int main()
{
    myThreadData.print("main() begin:");

    myThreadData.gName = "thread1 name";
    myThreadData.tName = "thread1 name";
    myThreadData.lName = "thread1 name";
    myThreadData.print("main() later, before use_itl:");
    // std::async(use_itl).wait();
    std::thread t{use_itl};
    t.join();
    myThreadData.print("main() end:");
}
