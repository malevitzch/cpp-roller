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

void DependencyGraph::toposort_dfs(fs::path path,
                  std::set<fs::path>& vis,
                  std::vector<fs::path>& res) {
  if(vis.contains(path)) return;
  vis.insert(path);
  for(fs::path next : data[path].dependencies)
    toposort_dfs(next, vis, res);
  res.push_back(path);

}

std::vector<std::filesystem::path> DependencyGraph::sorted() {
  std::vector<fs::path> res;
  std::set<fs::path> vis;
  for(auto&[path, _] : data)
    toposort_dfs(path, vis, res);
  return res;
}

DependencyGraph create_graph(std::vector<fs::path> sources) {
  DependencyGraph result;
  for(fs::path& source : sources) {
    crawl(source, result);
  }
  return result;
}
