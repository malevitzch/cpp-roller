#include <vector>
#include <fstream>
#include <sstream>
#include <regex>


static const std::regex include_re(R"==(#include[ ]*"([-A-Za-z0-9_.]*)")==", std::regex::extended);

std::vector<std::string> get_quote_includes(const std::string& buffer) {
  // This uses std::regex which is notoriously slow but the regex is linear
  // and simple enough that matching should be fast even on a bad backtracking
  // engine implementation (it has no "|" or suspicious "+" usage)

  std::vector<std::string> results;
  auto begin = std::sregex_iterator(buffer.begin(), buffer.end(), include_re);
  auto end = std::sregex_iterator();

  for(std::sregex_iterator it = begin; it != end; it++) {
    std::smatch match = *it;
    results.push_back(match[1].str());
  }
  return results;
}
std::string remove_quote_includes(const std::string& buffer) {
  return std::regex_replace(buffer, include_re, "");
}

std::vector<std::string> extract_includes(std::string filename) {
  std::ifstream input(filename);
  std::vector<std::string> result;


  std::stringstream buffer;
  buffer << input.rdbuf();
  std::string file = buffer.str();

  return get_quote_includes(file);
}
