#include "roller.hpp"
#include "grapher.hpp"

namespace fs = std::filesystem;


void roll(std::vector<fs::path> files) {
  DependencyGraph graph = create_graph(files);
  std::vector<fs::path> sorted = graph.sorted();
  // TODO: impl
}
