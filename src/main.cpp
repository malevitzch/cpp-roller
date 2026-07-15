#include "roller.hpp"
#include "common.hpp"
#include "argparse.hpp"

int run_main(int argc, args_t argv) {
  if constexpr(DEBUG) {
    ROLLER_CERR << "cpp-roller has been built in debugging mode, "
              << "build with build type RELEASE if this is not intended\n";
  }
  RollerConfig config;
  try {
    config = parse_args(argc, argv);
  } catch (CLIException& e) {
    ROLLER_CERR << STR("Error: ") << e.what() << STR("\n");
    return static_cast<int>(RollResult::UserError);
  }
  return static_cast<int>(roll(config));
}

#ifdef _WIN32
  #include <windows.h>
  int wmain(int argc, wchar_t** argv) {
    return run_main(argc, argv);
  }
#else
  int main(int argc, char** argv) {
    return run_main(argc, argv);
  }
#endif