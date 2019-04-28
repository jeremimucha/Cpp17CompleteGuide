#include "use_itl.hpp"
#include "inline_thread_local.hpp"

void use_itl()
{
    myThreadData.print("use_itl() begin:");

    myThreadData.gName = "thread2 name";
    myThreadData.tName = "thread2 name";
    myThreadData.lName = "thread2 name";

    myThreadData.print("use_itl() end:");
}
