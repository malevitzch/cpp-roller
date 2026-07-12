#include "roller.hpp"
#include "extractor.hpp"
#include "grapher.hpp"
#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <format>

namespace fs = std::filesystem;

void print_help() {
  std::cerr << "Usage: cpproll <filenames...>\n"
            << "Optional arguments:\n"
            << "\t-o <filename>   set output filename\n"
            << "\t-v              show version and exit\n"
            << "\t-h              show this help message and exit\n"
            << "\t-I <path>       add include directory\n";
}

RollResult roll(RollerConfig& config) {
  try {
    if(config.get_flag("version")) {
      std::cout << "cpp-roller version " << ROLLER_VERSION << "\n";
      return RollResult::Success;
    }
    if(config.get_flag("help")) {
      print_help();
      return RollResult::Success;
    }
    for(fs::path path : config.get_include_dirs()) {
      if(path.is_relative()) path = fs::current_path() / path;
      path = fs::weakly_canonical(path);
      if(!fs::exists(path)) {
        throw FileException("Include directory \n\t\"" + path.string() + "\"\ndoes not exist");
      }
    }
    std::vector<fs::path> sources(config.get_sources().begin(), config.get_sources().end());
    if(sources.empty()) {
      std::cerr << "No source files provided\n";
      print_help();
      return RollResult::UserError;
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
    return RollResult::Success;
  } catch (FileException& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return RollResult::FilesystemFailure;
  } catch (fs::filesystem_error& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return RollResult::FilesystemFailure;
  } catch (std::exception&) {
    return RollResult::UnexplainedFailure;
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

  std::istringstream iss{paths};
  std::string part;
  while(std::getline(iss, part, MULTIPATH_SEP)) {
    if(!part.empty()) {
      _include_paths.push_back(fs::weakly_canonical(part));
      if constexpr(DEBUG) {
        std::cerr << std::format("Added include directory: {}\n", fs::weakly_canonical(part).string());
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
