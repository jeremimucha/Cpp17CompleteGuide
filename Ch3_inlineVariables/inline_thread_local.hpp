#if !defined(INLINE_THREAD_LOCAL_H_)
#define INLINE_THREAD_LOCAL_H_

#include <iostream>
#include <string>

struct MyData {
    inline static std::string gName{"global"};              // unique per-program
    inline static thread_local std::string tName{"tls"};    // unique per-thread
    std::string lName{"local"};                             // for each object

    void print(std::string const& msg) const {
        std::cout << msg << "\n";
        std::cout << "- gName: " << gName << "\n";
        std::cout << "- tName: " << tName << "\n";
        std::cout << "- lName: " << lName << "\n";
    }
};

inline thread_local MyData myThreadData;    // one object per thread

#endif // INLINE_THREAD_LOCAL_H_
