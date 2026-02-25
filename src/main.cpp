#include <iostream>
#include <vector>
#include <filesystem>
#include "roller.hpp"


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

  RollerConfig config;

  for(int i = 0; i < args.size(); i++) {
    if(args[i] == "-o") {
      if(++i >= args.size()) {
        std::cerr << "-o option used without a filename argument\n";
        exit(1);
      }
      config.name(args[i]);
    }
    else if(args[i] == "-v") {
      config.version_flag();
    }
    else {
      config.add_source(args[i]);
    }
  }
  try {
    roll(config);
  } catch (std::filesystem::filesystem_error& e) {
    // TODO: better output
    std::cerr << e.what() << "\n";
  }
}
