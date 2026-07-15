#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP
#include <filesystem>
#include <set>
#include "common.hpp"

struct Includes {
  std::set<string_t> angle;
  std::set<string_t> quote;
};

Includes extract_includes(std::filesystem::path filename);
ostream_t& send_without_includes(std::filesystem::path filename, ostream_t& output);

#endif
