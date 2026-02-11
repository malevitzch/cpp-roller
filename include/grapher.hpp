#ifndef GRAPHER_HPP
#define GRAPHER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <filesystem>

class DependencyGraph {
private:
  struct FileData {
    std::string blob;
    std::set<std::filesystem::path> dependencies;
  };
  std::map<std::filesystem::path, FileData> data;
  void toposort_dfs(std::filesystem::path path,
                    std::set<std::filesystem::path>& vis,
                    std::vector<std::filesystem::path>& res);
public:
  bool add_file(std::filesystem::path filename);
  std::string get_blob(std::filesystem::path filename);
  void add_dependency(std::filesystem::path dependant, std::filesystem::path dependency);
  std::set<std::filesystem::path>& get_dependencies(std::filesystem::path file);
  std::vector<std::filesystem::path> sorted();
};

DependencyGraph create_graph(std::vector<std::filesystem::path> sources);

#endif
