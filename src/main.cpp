#include <iostream>
#include <vector>
#include <filesystem>
#include "roller.hpp"

int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "Not enough arguments\n";
    exit(1);
  }
  std::vector<std::string> args(argv + 1, argv + argc);
  std::vector<std::filesystem::path> sources;
  std::string output_name = "a.out";
  for(int i = 0; i < args.size(); i++) {
    if(args[i] == "-o") {
      i++;
      if(i >= args.size()) {
        std::cerr << "-o option used without a filename argument\n";
        exit(1);
      }
      output_name = args[i];
    } else {
      sources.push_back(args[i]);
    }
  }
  if(sources.empty()) {
    std::cerr << "Not enough source files\nUsage: cpproll <filenames...>\nOptional arguments:\n\t-o <filename>\n";
    exit(1);
  }
  try {
    roll(sources, output_name);
  } catch (std::filesystem::filesystem_error& e) {
    // TODO: better output
    std::cerr << e.what() << "\n";
  }
}
