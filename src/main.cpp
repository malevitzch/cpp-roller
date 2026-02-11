#include <iostream>
#include <vector>
#include <filesystem>
#include "extractor.hpp"
#include "roller.hpp"

int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "Not enough arguments\nUsage: cpproll <filename>\n";
    exit(1);
  }
  std::vector<std::string> args(argv + 1, argv + argc - 1);

  std::string filename = std::filesystem::absolute(argv[1]);
  std::vector<std::filesystem::path> sources = {filename};
  if(!std::filesystem::exists(filename)) {
    std::cerr << "The file \"" + filename + "\" doesn't exist\n";
    exit(1);
  }
  try {
    roll(sources, "a.out");
  } catch (std::filesystem::filesystem_error& e) {
    // TODO: better output
    std::cerr << e.what() << "\n";
  }
}
