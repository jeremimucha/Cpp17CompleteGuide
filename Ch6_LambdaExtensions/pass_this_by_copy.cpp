#include <chrono>
#include <iostream>
#include <string>
#include <thread>

/**
 * C++17 allows to pass `this` explicitly by copy. It also ensures no contradicting captures
 * of `this`.
 */

class Data {
public:
    template <typename... Args,
              typename = std::enable_if_t<std::is_constructible_v<std::string, Args...>>>
    Data(Args&&... args) : name_{std::forward<Args>(args)...}
    {
    }

    auto start_thread_with_copy_of_this() const
    {
        using namespace std::literals;
        return std::thread{[*this] { // capture `this` by copy
            std::this_thread::sleep_for(2s);
            std::cerr << "Hello from thread[" << std::this_thread::get_id() << "]: " << name_
                      << "\n";
        }};
    }

private:
    std::string name_{};
};

int main()
{
    std::thread t;
    {
        Data d{"scoped data"};
        t = d.start_thread_with_copy_of_this();
    }
    std::cerr << "outside of local scope\n";
    t.join();
}
