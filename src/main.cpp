#include <iostream>
#include <filesystem>
#include <getopt.h>
#include "roller.hpp"
#include "common.hpp"

int main(int argc, char** argv) {
  if constexpr(DEBUG) {
    std::cerr << "cpp-roller has been built in debugging mode, "
              << "build with build type RELEASE if this is not intended\n";
  }

  static option long_opts[] = {
    {"help",    no_argument,       nullptr, 'h'},
    {"version", no_argument,       nullptr, 'v'},
    {"output",  required_argument, nullptr, 'o'},
    {"include", required_argument, nullptr, 'I'},
    {nullptr,   0,                 nullptr, 0},
  };

  RollerConfig config;
  int opt;
  while ((opt = getopt_long(argc, argv, ":hvoI:", long_opts, nullptr)) != -1) {
    switch (opt) {
      case 'h':
        config.flag("help");
        break;
      case 'v':
        config.flag("version");
        break;
      case 'o':
        config.name(optarg);
        break;
      case 'I':
        // TODO: this should support a classic list of directories
        // separated by ':' or ';'
        config.add_include_directory(optarg);
        break;
      case ':':
        std::cerr << "The option \"" << char(optopt) << "\" requires an argument\n";
        exit(EXIT_FAILURE);
      case '?':
        std::cerr << "Unknown option: \"" << char(optopt) << "\"\n";
        exit(EXIT_FAILURE);
      default:
        std::cout << "Parse error: " << opt << "\n";
        exit(EXIT_FAILURE);
    }
  }
  for(int i = optind; i < argc; i++) {
    config.add_source(argv[i]);
  }

  try {
    roll(config);
  } catch (std::filesystem::filesystem_error& e) {
    // TODO: better output
    std::cerr << e.what() << "\n";
  }
  return 0;
}
