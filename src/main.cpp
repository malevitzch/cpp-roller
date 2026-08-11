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
    ROLLER_CERR << "Error: " << e.what() << "\n";
    return static_cast<int>(RollResult::UserError);
  }
  return static_cast<int>(roll(config));
}

#ifdef _WIN32
#include <filesystem>
#include <vector>
int wmain(int argc, wchar_t** argv) {
  std::vector<string_t> utf8_args;
  utf8_args.reserve(argc);
  for(int i = 0; i < argc; ++i) {
    const auto value = std::filesystem::path(argv[i]).u8string();
    utf8_args.emplace_back(reinterpret_cast<const char*>(value.data()), value.size());
  }

  std::vector<char*> args;
  args.reserve(argc);
  for(auto& arg : utf8_args) args.push_back(arg.data());
  return run_main(argc, args.data());
}
#else
int main(int argc, char** argv) {
  return run_main(argc, argv);
}
#endif
