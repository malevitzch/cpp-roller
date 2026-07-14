#include <iostream>
#include "roller.hpp"
#include "common.hpp"
#include "argparse.hpp"

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
