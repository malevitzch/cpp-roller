#include "roller.hpp"
#include "extractor.hpp"
#include "grapher.hpp"
#include "common.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void print_help() {
  std::cerr << "Usage: cpproll <filenames...>\n"
            << "Optional arguments:\n"
            << "\t-o <filename>   set output filename\n"
            << "\t-v              show version and exit\n"
            << "\t-h              show this help message and exit\n"
            << "\t-I <path>       add include directory\n";
}

void roll(RollerConfig& config) {
  if(config.get_flag("version")) {
    std::cout << "cpp-roller version " << ROLLER_VERSION << "\n";
    exit(EXIT_SUCCESS);
  }
  if(config.get_flag("help")) {
    print_help();
    exit(EXIT_SUCCESS);
  }
  std::vector<fs::path> sources(config.get_sources().begin(), config.get_sources().end());
  if(sources.empty()) {
    std::cerr << "No source files provided\n";
    print_help();
    exit(EXIT_FAILURE);
  }
  DependencyGraph graph(config);
  std::vector<fs::path> sorted = graph.sorted();
  std::ofstream out(config.get_output_name());
  for(std::string lib : graph.get_angle_includes()) {
    out << "#include <" << lib << ">\n";
  }
  for(fs::path path : sorted) {
    send_without_includes(path, out);
  }
}

RollerConfig& RollerConfig::name(std::string new_name) {
  _output_name = new_name;
  return *this;
}
RollerConfig& RollerConfig::add_source(std::string source) {
  _sources.insert(source);
  return *this;
}

RollerConfig& RollerConfig::add_include_directories(std::string paths) {
  #ifdef _WIN32
    constexpr char sep = ';';
  #else
    constexpr char sep = ':';
  #endif

  std::istringstream iss{paths};
  std::string part;
  while (std::getline(iss, part, sep)) {
    if (!part.empty()) {
      _include_paths.push_back(fs::weakly_canonical(part));
      if constexpr(DEBUG) {
        std::cout << "Added include directory: " << fs::weakly_canonical(part) << "\n";
      }
    }
  }
  return *this;
}
RollerConfig& RollerConfig::flag(std::string name, bool value) {
    _flags[name] = value;
    return *this;
}
RollerConfig& RollerConfig::flag(std::string name) {
  return flag(name, true);
}

const std::set<std::filesystem::path>& RollerConfig::get_sources() { return _sources; }
std::string RollerConfig::get_output_name() { return _output_name; }
bool RollerConfig::get_flag(std::string name) { return _flags[name]; }
const std::vector<std::filesystem::path>& RollerConfig::get_include_dirs() { return _include_paths; }
