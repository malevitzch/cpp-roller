#ifndef GRAPHER_HPP
#define GRAPHER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

class DependencyGraph {
private:
  struct FileData {
    std::string blob;
    std::set<std::string> dependencies;
  };
  std::map<std::string, FileData> data;
public:
  bool add_file(std::string filename);
  void add_dependency(std::string dependant, std::string dependency);
  std::set<std::string>& get_dependencies(std::string file);
};

DependencyGraph create_graph(std::vector<std::string> sources);

#endif
