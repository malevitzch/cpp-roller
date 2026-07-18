#ifndef GRAPHER_HPP
#define GRAPHER_HPP

#include <vector>
#include <map>
#include <set>
#include <filesystem>

#include "common.hpp"

class DependencyGraph;
class RollerConfig;

// First is file size, second is the real hash.
// This dramatically reduces the chances of a hash collision.
using FileHash = std::pair<size_t, size_t>;

struct FileData {
  std::filesystem::path path;
  FileHash hash;
  std::set<FileHash> dependencies;
  FileData(std::filesystem::path path, FileHash hash);
  FileData() = default;
};

FileHash hash_file(std::filesystem::path path);

class DependencyGraph {
private:
  RollerConfig& config;
  std::set<string_t> angle_includes;
  std::map<FileHash, FileData> files;
  std::map<std::filesystem::path, FileHash> lookup; // To avoid recompute.
  void toposort_dfs(FileHash hash,
                    std::set<FileHash>& vis,
                    std::vector<std::filesystem::path>& res);
  FileHash get_file_hash(std::filesystem::path path);
public:
  bool add_file(std::filesystem::path filename);

  std::vector<std::filesystem::path> sorted();
  std::set<string_t>& get_angle_includes();

  DependencyGraph(RollerConfig& config);
};

#endif
