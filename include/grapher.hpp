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
  std::map<std::string, FileData> data;
public:
  bool add_file(std::string filename);
  void add_dependency(std::filesystem::path dependant, std::filesystem::path dependency);
  std::set<std::filesystem::path>& get_dependencies(std::string file);
};

DependencyGraph create_graph(std::vector<std::string> sources);

#endif
