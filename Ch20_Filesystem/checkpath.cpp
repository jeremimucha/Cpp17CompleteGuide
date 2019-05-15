#include <filesystem>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path>\n";
        return EXIT_FAILURE;
    }

    std::filesystem::path p{argv[1]};  // filesystem path, might not exist
    if (std::filesystem::is_regular_file(p)) {
        std::cout << "\"" << p << "\" is a file with " << std::filesystem::file_size(p)
                  << " bytes\n";
    }
    else if (is_directory(p)) { // ADL - could have also been used above,
        std::cout << "\"" << p << "\" is a directory containing:\n";
        for (auto& e : std::filesystem::directory_iterator{p}) {
            // using .path().string() prevents automatically quoting and escaping backslashes
            // when printing the path:
            std::cout << " \"" << e.path().string() << "\"\n";
        }
    }
    else if (exists(p)) {
        std::cout << "\"" << p << "\" exists, is a special file\n";
    }
    else {
        std::cout << "path \"" << p << "\" does not exist\n";
    }
}
