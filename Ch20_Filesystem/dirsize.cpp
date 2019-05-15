#include <execution>  // for the execution policy
#include <filesystem>
#include <iostream>
#include <numeric>  // for transform_reduce()
#include <vector>

int main(int argc, char* argv[])
{
    namespace fs = std::filesystem;
    // root directory is passed as command line argument:
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path> \n";
        return EXIT_FAILURE;
    }
    fs::path root{argv[1]};

    // init list of all file paths in passed file tree:
    std::vector<fs::path> paths;
    try {
        fs::recursive_directory_iterator dirpos{root};
        std::copy(begin(dirpos), end(dirpos), std::back_inserter(paths));
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // accumulate size of all regular files:
    auto const sz =
        std::transform_reduce(std::execution::par,         // parallel execution
                              cbegin(paths), cend(paths),  // range
                              std::uintmax_t{0},           // initial value
                              std::plus<>(),               // accumulate ...
                              [](const fs::path& p) {      //  file size if regular file
                                  if (is_regular_file(p)) { return file_size(p); }
                                  return std::uintmax_t{0};
                              });
    std::cout << "size of all " << paths.size() << " regular files: " << sz << '\n';
}
