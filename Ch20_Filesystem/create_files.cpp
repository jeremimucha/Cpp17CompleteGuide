#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>


int main()
{
    namespace fs = std::filesystem;
    try {
        fs::path test_dir{"temp/test"};
        create_directories(test_dir);

        auto test_file{test_dir/"data.txt"};
        std::ofstream data_file{test_file};
        if (!data_file.is_open()) {
            std::cerr << "Failed to open \"" << test_file << "\"\n";
            std::exit(EXIT_FAILURE);
        }
        data_file << "The answer is 42\n";

        // create a symbolic link
        // The first argument is the path from the view of the created link (second argument)
        // - it's relative to the parent_path of the second argument
        try {
            create_directory_symlink("test", test_dir.parent_path() / "slink");
        } catch (fs::filesystem_error& e) {
            std::cerr << e.what() << "\n"
                << "code: " << e.code() << "\n"
                << "path1: " << e.path1() << "\n"
                << "path2: " << e.path2() << "\n";
        }

        // recursively list all files
        std::cout << fs::current_path().string() << ":\n";
        auto const iter_opts{fs::directory_options::follow_directory_symlink};
        for (auto const& e : fs::recursive_directory_iterator{".", iter_opts}) {
            std::cout << "  " << e.path().lexically_normal().string() << "\n";
        }
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        std::cerr << "\te.path1(): \"" << e.path1().string() << "\"\n";
    }
}
