#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path> \n";
        return EXIT_FAILURE;
    }

    // instead of chaining if-else statements we can directly query and switch on the type of
    // fs::path:
    switch (fs::path p{argv[1]}; status(p).type()) {
    case fs::file_type::not_found:
        std::cerr << "path \"" << p.string() << "\" does not exist\n";
        break;
    case fs::file_type::regular:
        std::cerr << '"' << p.string() << "\" is a regular file, size = " << file_size(p)
                  << " bytes\n";
        break;
    case fs::file_type::directory:
        std::cerr << '"' << p.string() << "\" is a directory containing:\n";
        for (auto& e : std::filesystem::directory_iterator{p}) {
            std::cerr << " " << e.path().string() << '\n';
        }
        break;
    default:
        std::cerr << '"' << p.string() << "\" is a special file\n";
        break;
    }
}
