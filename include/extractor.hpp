#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP
#include <string>
#include <filesystem>
#include <set>

struct Includes {
  std::set<std::string> angle;
  std::set<std::string> quote;
};

Includes extract_includes(std::filesystem::path filename);
std::ostream& send_without_includes(std::filesystem::path filename, std::ostream& output);

#endif
