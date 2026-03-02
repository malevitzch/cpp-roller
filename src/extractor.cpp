#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>


static const std::regex include_re(R"==(#include[ ]*"([-A-Za-z0-9_.]*)")==", std::regex::extended);

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

std::string remove_quote_includes(std::string buffer) {
  return std::regex_replace(buffer, include_re, "");
}

std::string extract_blob(std::string filename) {
  return remove_quote_includes(file_to_string(filename));
}

std::vector<std::string> extract_includes(std::string filename) {

  return get_quote_includes(file_to_string(filename));
}
