#include "grapher.hpp"
#include "extractor.hpp"

#include <filesystem>
namespace fs = std::filesystem;

// Assumes 'filename' is an absolute path
bool DependencyGraph::add_file(std::string filename) {
  if(!data.contains(filename)) return false;
  std::string blob = extract_blob(filename);
  data[filename].blob = blob;
  return true;
}

void DependencyGraph::add_dependency(std::string dependant, std::string dependency) {
  data[dependant].dependencies.insert(dependency);
}

std::set<std::string>& DependencyGraph::get_dependencies(std::string file) {
  return data[file].dependencies;
}

void crawl(const std::string& source, DependencyGraph& graph) {
  // TODO: impl
}

DependencyGraph create_graph(std::vector<std::string> sources) {
  DependencyGraph result;
  for(std::string& source : sources) {
    crawl(source, result);
  }
  return result;
}
