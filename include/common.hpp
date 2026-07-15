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
#include <string>
#include <regex>

#ifdef _WIN32
  #include <windows.h>
  #define FMTPATH(x) x.wstring()
  #define STR_(x) L##x
  #define STR(x) STR_(x)
  #define STRLEN(x) wcslen(x)
  constexpr wchar_t MULTIPATH_SEP = ';';
  using string_t = std::wstring;
  using regex_t = std::wregex;
  using match_t = std::wsmatch;
  using ostream_t = std::wostream;
  using ofstream_t = std::wofstream;
  using ifstream_t = std::wifstream;
  using istringstream_t = std::wistringstream;
  template<typename T>
  inline string_t to_string_t(const T& val) {
    return std::to_wstring(val);
  }
  #define ROLLER_CERR std::wcerr
#else
  #define FMTPATH(x) x.string()
  #define STR(x) x
  #define STRLEN(x) strlen(x)
  using string_t = std::string;
  constexpr char MULTIPATH_SEP = ':';
  using regex_t = std::regex;
  using match_t = std::smatch;
  using ostream_t = std::ostream;
  using ofstream_t = std::ofstream;
  using ifstream_t = std::ifstream;
  using istringstream_t = std::istringstream;
  template<typename T>
  inline string_t to_string_t(const T& val) {
    return std::to_string(val);
  }
  #define ROLLER_CERR std::cerr
#endif

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