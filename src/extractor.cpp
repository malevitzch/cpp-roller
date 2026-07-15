#include "common.hpp"
#include "extractor.hpp"

#include <filesystem>
#include <optional>
#include <regex>
#include <set>


static const regex_t quote_include_re(
  STR(R"==(^[ \t]*#include[ \t]*"([^"]+)"[ \t]*\n?)=="),
  std::regex::ECMAScript
);
static const regex_t angle_include_re(
  STR(R"==(^[ \t]*#include[ \t]*<([^>]+)>[ \t]*\n?)=="),
  std::regex::ECMAScript
);

std::optional<string_t> get_quote_include(string_t line) {
  match_t match;
  if(std::regex_search(line, match, quote_include_re)) return match[1];
  return std::nullopt;
}
std::optional<string_t> get_angle_include(string_t line) {
  match_t match;
  if(std::regex_search(line, match, angle_include_re)) return match[1];
  return std::nullopt;
}

bool is_include(string_t line) {
  return std::regex_search(line, quote_include_re) || std::regex_search(line, angle_include_re);
}

Includes extract_includes(std::filesystem::path filename) {
  ifstream_t input(filename);

  Includes includes;
  string_t line;
  while(std::getline(input, line)) {
    std::optional<string_t> inc;
    if((inc = get_quote_include(line))) {
      includes.quote.insert(*inc);
    }
    else if((inc = get_angle_include(line))) {
      includes.angle.insert(*inc);
    }
  }
  return includes;
}

ostream_t& send_without_includes(std::filesystem::path filename, ostream_t& output) {
  ifstream_t input(filename);
  string_t line;
  while(std::getline(input, line)) {
    if(!is_include(line)) output << line << '\n';
  }
  return output;
}


