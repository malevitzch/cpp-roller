#include "extractor.hpp"
#include "common.hpp"

#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>
#include <set>


static const std::regex quote_include_re(R"==(#include[ ]*"([-A-Za-z0-9_/.]*)"[ ]*\n?)==", std::regex::ECMAScript);
static const std::regex angle_include_re(R"==(#include[ ]*<([-A-Za-z0-9_/.]*)>[ ]*\n?)==", std::regex::ECMAScript);

std::string file_to_string(std::string filename) {
  std::ifstream input(filename);
  if(!std::filesystem::exists(filename)) {
    std::string msg = "Failed to load file \"" + filename + "\"";
    throw std::filesystem::filesystem_error(msg, std::make_error_code(std::errc::no_such_file_or_directory));
  }

  std::stringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

inline std::vector<std::string> get_includes(const std::regex& re, const std::string& buffer) {
  // This uses std::regex which is notoriously slow but the regex is linear
  // and simple enough that matching should be fast even on a bad backtracking
  // engine implementation (it has no "|" or suspicious "+" usage)
  std::vector<std::string> results;
  auto begin = std::sregex_iterator(buffer.begin(), buffer.end(), re);
  auto end = std::sregex_iterator();

  for(std::sregex_iterator it = begin; it != end; it++) {
    std::smatch match = *it;
    results.push_back(match[1].str());
  }
  return results;
}

std::vector<std::string> get_quote_includes(const std::string& buffer) {
  return get_includes(quote_include_re, buffer);
}
std::vector<std::string> get_angle_includes(const std::string& buffer) {
  return get_includes(angle_include_re, buffer);
}

std::string remove_quote_includes(std::string buffer) {
  return std::regex_replace(buffer, quote_include_re, "");
}
std::string remove_angle_includes(std::string buffer) {
  return std::regex_replace(buffer, angle_include_re, "");
}

std::string extract_blob(std::string filename) {
  return remove_angle_includes(remove_quote_includes(file_to_string(filename)));
}

std::vector<std::string> extract_includes(std::string filename, std::set<std::string>& angle_include_set) {
  auto includes = get_quote_includes(file_to_string(filename));
  auto angle_includes = get_angle_includes(file_to_string(filename));
  angle_include_set.insert(angle_includes.begin(), angle_includes.end());

  if constexpr(DEBUG) {
    std::cerr << "Found " << includes.size() << " includes in \"" << filename << "\":\n";
    for(auto& inc : includes) {
      std::cerr << "\t" << inc << "\n";
    }
  }
  return includes;
}
