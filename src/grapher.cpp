#include "grapher.hpp"
#include "extractor.hpp"
#include "common.hpp"
#include "roller.hpp"

#include <filesystem>
#include <fstream>
#include <format>

#include <iostream>

namespace fs = std::filesystem;

FileData::FileData(fs::path path, FileHash hash) : path(path), hash(hash) {}

FileHash hash_file(std::filesystem::path path) {
  if(!fs::exists(path)) {
    throw FileException(std::format(STR("No such file as \"{}\""), FMTPATH(path)));
  }
  std::ifstream file(path);
  std::string line;
  size_t hash = 0;
  while(std::getline(file, line)) {
    hash = hash_combine(std::hash<std::string>{}(line), hash);
  }
  return {fs::file_size(path), hash};
}

FileHash DependencyGraph::get_file_hash(fs::path path) {
  path = fs::weakly_canonical(path);
  if(!lookup.contains(path)) {
    FileHash hash = hash_file(path);
    lookup[path] = hash;
  }
  return lookup[path];
}

bool DependencyGraph::add_file(fs::path filepath) {
  filepath = fs::weakly_canonical(filepath);
  FileHash hash = get_file_hash(filepath);
  lookup[filepath] = hash;
  if(files.contains(hash)) return false;

  files[hash] = FileData(filepath, lookup.at(filepath));
  Includes includes = extract_includes(filepath);
  for(const auto& inc : includes.angle)
    angle_includes.insert(inc);

  fs::path dir = filepath.parent_path();
  std::vector<fs::path> dependencies;
  for(const auto& p : includes.quote) {
    bool found = false;
    for(const auto& inc_dir : config.get_include_dirs()) {
      fs::path dep = fs::weakly_canonical(inc_dir / p);
      if(fs::exists(dep)) {
        dependencies.push_back(dep);
        found = true;
        break;
      }
    }
    if(!found) {
      fs::path dep = fs::weakly_canonical(dir / p);
      if(!fs::exists(dep)) {
        throw FileException(std::format(STR("Dependency \"{}\" required by \"{}\" does not exist"),
                                        FMTPATH(dep), FMTPATH(filepath)));
      }
      dependencies.push_back(fs::weakly_canonical(dir / p));
    }
  }
  if constexpr(DEBUG) {
    std::cout << dependencies.size() << " dependencies found for " << filepath << '\n';
  }
  for(const auto& dep : dependencies) {
    FileHash dep_hash = get_file_hash(dep);
    if(!files.contains(dep_hash)) add_file(dep);
    files[hash].dependencies.insert(dep_hash);
  }

  return true;
}

void DependencyGraph::toposort_dfs(FileHash hash,
                  std::set<FileHash>& vis,
                  std::vector<fs::path>& res) {
  if(vis.contains(hash)) return;
  vis.insert(hash);
  for(FileHash next : files[hash].dependencies)
    toposort_dfs(next, vis, res);
  res.push_back(files[hash].path);
}

std::vector<fs::path> DependencyGraph::sorted() {
  std::vector<fs::path> res;
  std::set<FileHash> vis;
  for(auto&[hash, _] : files)
    toposort_dfs(hash, vis, res);
  if constexpr(DEBUG) {
    std::cout << res.size() << " files sorted\n";
  }
  return res;
}

std::set<string_t>& DependencyGraph::get_angle_includes() {
  return angle_includes;
}

DependencyGraph::DependencyGraph(RollerConfig& config) : config(config) {
  for(const fs::path& source : config.get_sources())
    add_file(source);
}