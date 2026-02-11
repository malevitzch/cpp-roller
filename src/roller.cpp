#include "roller.hpp"
#include "grapher.hpp"

#include <fstream>

namespace fs = std::filesystem;


void roll(std::vector<fs::path> files, std::filesystem::path output_filename) {
  DependencyGraph graph = create_graph(files);
  std::vector<fs::path> sorted = graph.sorted();
  std::ofstream out(output_filename);
  for(fs::path path : sorted) {
    out << graph.get_blob(path);
  }
}
