#include "grapher.hpp"
#include "extractor.hpp"

#include <filesystem>
namespace fs = std::filesystem;

// Assumes 'filename' is absolute
void add_file(std::string filename) {
  std::string blob = extract_blob(filename);
  /*std::vector<std::string> includes = extract_includes(filename);
  for(std::string& file : includes) {
    fs::path path = fs::absolute(file);
  }*/
  // TODO: impl
}

void DependencyGraph::add_dependency(std::string dependant, std::string dependency) {
  data[dependant].dependencies.insert(dependency);
}

std::set<std::string>& DependencyGraph::get_dependencies(std::string file) {
  return data[file].dependencies;
}
