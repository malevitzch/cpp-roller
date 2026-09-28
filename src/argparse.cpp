#include "argparse.hpp"
#include "common.hpp"

#include <format>
#include <string_view>

namespace {
  struct Option {
    std::string long_name;
    std::string short_name;
    bool takes_argument;
  };

  // TODO: allow clustered options eventually
  const Option* find_option(std::string_view name) {
    static constexpr Option options[] = {
      {"help",          "h", false},
      {"version",       "v", false},
      {"output",        "o", true },
      {"include",       "I", true },
      {"force",         "f", false},
      {"no-overwrite",  "n", false},
    };
    for(const auto& option : options) {
      if(name == option.long_name || name == option.short_name) return &option;
    }
    return nullptr;
  }
}

RollerConfig parse_args(int argc, args_t argv) {
  RollerConfig config;
  for(int i = 1; i < argc; ++i) {
    string_t argument = argv[i];
    if(argument.size() < 2 || argument[0] != '-') {
      config.add_source(argument);
      continue;
    }

    const bool long_option = argument.size() >= 3 && argument[1] == '-';
    const std::string_view name(argument.data() + (long_option ? 2 : 1),
                                argument.size() - (long_option ? 2 : 1));
    const Option* option = find_option(name);
    if(!option) {
      throw CLIException(std::format("Unknown option \"{}\"", name));
    }
    if(option->takes_argument && i + 1 >= argc) {
      throw CLIException(std::format("Option \"{}\" requires an argument", name));
    }
    static std::set<std::string> flag_options = {
      "help",
      "version",
      "force",
      "no-overwrite"
    };
    if(flag_options.contains(option->long_name)) {
      config.flag(option->long_name);
    } else if(option->long_name == "output") {
      config.name(argv[++i]);
    } else if(option->long_name == "include") {
      config.add_include_directories(argv[++i]);
    } else {
      throw DeveloperException("Unknown option snuck past parsing stage.");
    }
  }
  return config;
}
