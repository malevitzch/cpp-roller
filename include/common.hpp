#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef DEBUG
#define DEBUG false
#endif

#ifndef ROLLER_VERSION
#define ROLLER_VERSION "???"
#endif

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

using string_t = std::string;
using roller_regex_t = std::regex;
using match_t = std::smatch;
using ostream_t = std::ostream;
using ofstream_t = std::ofstream;
using ifstream_t = std::ifstream;
using istringstream_t = std::istringstream;

#ifdef _WIN32
constexpr char MULTIPATH_SEP = ';';
#else
constexpr char MULTIPATH_SEP = ':';
#endif

// Construct a native filesystem path from UTF-8, including on Windows where
// filesystem paths use UTF-16 internally.
inline std::filesystem::path path_from_utf8(const string_t& value) {
  const auto* data = reinterpret_cast<const char8_t*>(value.data());
  return std::filesystem::path(std::u8string(data, data + value.size()));
}

inline string_t path_to_utf8(const std::filesystem::path& value) {
  const auto utf8 = value.u8string();
  return {reinterpret_cast<const char*>(utf8.data()), utf8.size()};
}

#define STR(x) x
#define ROLLER_CERR std::cerr

template<typename T>
inline string_t to_string_t(const T& val) {
  return std::to_string(val);
}

std::size_t hash_combine(std::size_t h1, std::size_t h2);

// Handles improper CLI usage
class CLIException {
public:
  explicit CLIException(const string_t& message);
  string_t what() const noexcept;
private:
  string_t message;
};

// Handles missing, unreadable, or otherwise inaccessible files
class FileException {
public:
  explicit FileException(const string_t& message);
  string_t what() const noexcept;
private:
  string_t message;
};
#endif
