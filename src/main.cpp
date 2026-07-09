#include <iostream>
#include <getopt.h>
#include <format>
#include "roller.hpp"
#include "common.hpp"

RollerConfig parse_args(int argc, char** argv) {
  static option long_opts[] = {
    {"help",    no_argument,       nullptr, 'h'},
    {"version", no_argument,       nullptr, 'v'},
    {"output",  required_argument, nullptr, 'o'},
    {"include", required_argument, nullptr, 'I'},
    {nullptr,   0,                 nullptr, 0},
  };

  RollerConfig config;
  int opt;
  while ((opt = getopt_long(argc, argv, ":hvo:I:", long_opts, nullptr)) != -1) {
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
        config.add_include_directories(optarg);
        break;
      case ':': {
        throw CLIException(std::format("The option \"{}\" requires an argument", char(optopt)));
      }
      case '?':
        throw CLIException(std::format("Unknown option \"{}\"", char(optopt)));
      default:
        throw CLIException(std::format("Unknown error while parsing option \"{}\"", char(optopt)));
    }
  }
  for(int i = optind; i < argc; i++) {
    config.add_source(argv[i]);
  }
  return config;
}


int main(int argc, char** argv) {
  if constexpr(DEBUG) {
    std::cerr << "cpp-roller has been built in debugging mode, "
              << "build with build type RELEASE if this is not intended\n";
  }
  RollerConfig config;
  try {
    config = parse_args(argc, argv);
  } catch (CLIException& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return static_cast<int>(RollResult::UserError);
  }
  return static_cast<int>(roll(config));
}
