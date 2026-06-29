#ifndef GRAPHER_HPP
#define GRAPHER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <filesystem>

// First is file size, second is the real hash.
// This dramatically reduces the chances of a hash collision.
using FileHash = std::pair<size_t, size_t>;

class DependencyGraph;
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
  std::set<std::string> angle_includes;
  std::map<FileHash, FileData> files;
  std::map<std::filesystem::path, FileHash> lookup; // To avoid recompute.
  void toposort_dfs(FileHash hash,
                    std::set<FileHash>& vis,
                    std::vector<std::filesystem::path>& res);
  FileHash get_file_hash(std::filesystem::path path);
public:
  bool add_file(std::filesystem::path filename);
  
  std::vector<std::filesystem::path> sorted();
  std::set<std::string>& get_angle_includes();

  DependencyGraph(std::vector<std::filesystem::path> sources);
};

#endif
