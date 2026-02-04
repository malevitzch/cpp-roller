#include <vector>
#include <fstream>
#include <sstream>
#include "jpcre2.hpp"

std::vector<std::string> extract_includes(std::string filename) {
  std::ifstream input(filename);
  std::stringstream buffer;
  buffer << input.rdbuf();
  std::string file = buffer.str();
  //pcrecpp::RE inc_re("include\"([A-Za-z0-9._-])\"");
  std::vector<std::string> result;
  return {};
}
