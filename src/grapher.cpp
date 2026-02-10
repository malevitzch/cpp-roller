#include "grapher.hpp"
#include "extractor.hpp"

#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;

// Assumes 'filename' is an absolute path
bool DependencyGraph::add_file(std::string filename) {
  if(data.contains(filename)) return false;
  std::string blob = extract_blob(filename);
  data[filename].blob = blob;
  return true;
}

void DependencyGraph::add_dependency(fs::path dependant, fs::path dependency) {
  data[dependant].dependencies.insert(dependency);
}

std::set<fs::path>& DependencyGraph::get_dependencies(std::string file) {
  return data[file].dependencies;
}

void crawl(const fs::path& source, DependencyGraph& graph) {
  fs::path dir = source.parent_path();
  if(!graph.add_file(source)) return; // Do not re-enter already visited files
  std::vector<std::string> includes = extract_includes(source);

  auto view = includes | std::views::transform([&](const std::string& p) {return dir / p;});
  std::vector<fs::path> dependencies(view.begin(), view.end());

  for(fs::path& dependency : dependencies) {
    graph.add_dependency(source, dependency);
    crawl(dependency, graph);
  }
}

DependencyGraph create_graph(std::vector<std::string> sources) {
  DependencyGraph result;
  for(std::string& source : sources) {
    crawl(source, result);
  }
  return result;
}
