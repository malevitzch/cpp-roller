#include "roller.hpp"
#include "extractor.hpp"
#include "grapher.hpp"
#include "common.hpp"

#include <filesystem>
#include <iostream>
#include <format>

namespace fs = std::filesystem;

void print_help() {
  ROLLER_CERR << "Usage: cpproll <filenames...>\n"
              << "Optional arguments:\n"
              << "\t-o <filename>   set output filename\n"
              << "\t-v              show version and exit\n"
              << "\t-h              show this help message and exit\n"
              << "\t-I <path>       add include directory\n"
              << "\t-f              force overwrite of output file\n"
              << "\t-n              do not overwrite output file if it exists";
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
        throw FileException(std::format("Include directory \n\t\"{}\"\ndoes not exist", path_to_utf8(path)));
      }
    }
    std::vector<fs::path> sources(config.get_sources().begin(), config.get_sources().end());
    if(sources.empty()) {
      ROLLER_CERR << "No source files provided\n";
      print_help();
      return RollResult::UserError;
    }
    DependencyGraph graph(config);
    std::vector<fs::path> sorted = graph.sorted();
    ofstream_t out(path_from_utf8(config.get_output_name()));
    if(!out) {
      throw FileException(std::format("Failed to open output file \"{}\"",
                          path_to_utf8(config.get_output_name())));
    }
    for(string_t lib : graph.get_angle_includes()) {
      out << "#include <" << lib << ">\n";
    }
    for(fs::path path : sorted) {
      send_without_includes(path, out);
    }
    return RollResult::Success;
  } catch(FileException& e) {
    ROLLER_CERR << "Error: " << e.what() << "\n";
    return RollResult::FilesystemFailure;
  } catch(fs::filesystem_error& e) {
    ROLLER_CERR << "Error: " << e.what() << "\n";
    return RollResult::FilesystemFailure;
  } catch(DeveloperException &e) {
    ROLLER_CERR << "An internal error has occured."
                << "Please report this issue to the developers.\n"
                << "Error: " << e.what() << "\n";
    return RollResult::DeveloperFailure;
  } catch(std::exception&) {
    return RollResult::UnexplainedFailure;
  }
}

RollerConfig& RollerConfig::name(string_t new_name) {
  _output_name = new_name;
  return *this;
}
RollerConfig& RollerConfig::add_source(string_t source) {
  _sources.insert(path_from_utf8(source));
  return *this;
}

RollerConfig& RollerConfig::add_include_directories(string_t paths) {
  istringstream_t iss{paths};
  string_t part;
  while(std::getline(iss, part, MULTIPATH_SEP)) {
    if(!part.empty()) {
      _include_paths.push_back(fs::weakly_canonical(path_from_utf8(part)));
      if constexpr(DEBUG) {
        ROLLER_CERR << std::format("Added include directory: {}\n", path_to_utf8(fs::weakly_canonical(path_from_utf8(part))));
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
string_t RollerConfig::get_output_name() { return _output_name; }
bool RollerConfig::get_flag(std::string name) { return _flags[name]; }
const std::vector<std::filesystem::path>& RollerConfig::get_include_dirs() { return _include_paths; }
