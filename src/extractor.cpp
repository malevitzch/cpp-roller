#include "extractor.hpp"
#include "common.hpp"

#include <filesystem>
#include <optional>
#include <fstream>
#include <regex>
#include <set>


static const std::regex quote_include_re(R"==(#include[ ]*"([-A-Za-z0-9_/.]*)"[ ]*\n?)==", std::regex::ECMAScript);
static const std::regex angle_include_re(R"==(#include[ ]*<([-A-Za-z0-9_/.]*)>[ ]*\n?)==", std::regex::ECMAScript);

std::optional<std::string> get_quote_include(std::string line) {
  std::smatch match;
  if(std::regex_search(line, match, quote_include_re)) return match[1];
  return std::nullopt;  
}
std::optional<std::string> get_angle_include(std::string line) {
  std::smatch match;
  if(std::regex_search(line, match, angle_include_re)) return match[1];
  return std::nullopt;
}

bool is_include(std::string line) {
  return std::regex_search(line, quote_include_re) || std::regex_search(line, angle_include_re);
}

Includes extract_includes(std::filesystem::path filename) {
  std::ifstream input(filename);

  Includes includes;
  std::string line;
  while(std::getline(input, line)) {
    std::optional<std::string> inc;
    if((inc = get_quote_include(line))) {
      includes.quote.insert(*inc);
    }
    else if((inc = get_angle_include(line))) {
      includes.angle.insert(*inc);
    }
  }
  return includes;
}

std::ostream& send_without_includes(std::filesystem::path filename, std::ostream& output) {
  std::ifstream input(filename);
  std::string line;
  while(std::getline(input, line)) {
    if(!is_include(line)) output << line << '\n';
  }
  return output;
}


