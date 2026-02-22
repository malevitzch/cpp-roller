#include <iostream>
#include <vector>
#include <filesystem>
#include "roller.hpp"

#ifndef ROLLER_VERSION
#define ROLLER_VERSION "???"
#endif

int main(int argc, char** argv) {
  #ifdef DEBUG
  std::cerr << "cpp-roller is running in debugging mode, "
            << "disable debug if it's not intended\n";
  #endif
  if(argc < 2) {
    std::cerr << "Not enough arguments\n";
    exit(1);
  }
  std::vector<std::string> args(argv + 1, argv + argc);

  std::vector<std::filesystem::path> sources = {};
  std::string output_name = "a.out";
  bool v = false; // Whether or not the "-v" flag has been input

  for(int i = 0; i < args.size(); i++) {
    if(args[i] == "-o") {
      if(++i >= args.size()) {
        std::cerr << "-o option used without a filename argument\n";
        exit(1);
      }
      output_name = args[i];
    }
    else if(args[i] == "-v") {
      std::cout << "cpp-roller version " << ROLLER_VERSION << "\n";
      exit(0);
    }
    else {
      sources.push_back(args[i]);
    }
  }
  if(sources.empty()) {
    std::cerr << "Not enough source files\nUsage: cpproll <filenames...>\n"
              << "Optional arguments:\n"
              << "\t-o <filename>\n"
              << "\t-v\n";
    exit(1);
  }
  try {
    roll(sources, output_name);
  } catch (std::filesystem::filesystem_error& e) {
    // TODO: better output
    std::cerr << e.what() << "\n";
  }
}
