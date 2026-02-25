#include "roller.hpp"
#include "grapher.hpp"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;


#ifndef ROLLER_VERSION
#define ROLLER_VERSION "???"
#endif

void roll(RollerConfig& config) {
  if(config.get_version_flag()) {
    std::cout << "cpp-roller version " << ROLLER_VERSION << "\n";
    exit(0);
  }
  std::vector<fs::path> sources(config.get_sources().begin(), config.get_sources().end());
  if(sources.empty()) {
    std::cerr << "Not enough source files\nUsage: cpproll <filenames...>\n"
              << "Optional arguments:\n"
              << "\t-o <filename>\n"
              << "\t-v\n";
    exit(1);
  }
  DependencyGraph graph = create_graph(sources);
  std::vector<fs::path> sorted = graph.sorted();
  std::ofstream out(config.get_output_name());
  for(fs::path path : sorted) {
    out << graph.get_blob(path);
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
RollerConfig& RollerConfig::version_flag() {
  _version = true;
  return *this;
}

const std::set<std::filesystem::path>& RollerConfig::get_sources() { return _sources; }
std::string RollerConfig::get_output_name() { return _output_name; }
bool RollerConfig::get_version_flag() { return _version; }
