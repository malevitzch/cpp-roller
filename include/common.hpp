#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef DEBUG
#define DEBUG false
#endif

#ifndef ROLLER_VERSION
#define ROLLER_VERSION "???"
#endif

#include <cstddef>
#include <exception>
#include <string>

#ifdef _WIN32
constexpr char MULTIPATH_SEP = ';';
#else
constexpr char MULTIPATH_SEP = ':';
#endif

std::size_t hash_combine(std::size_t h1, std::size_t h2);

// Handles improper CLI usage
class CLIException : public std::exception {
public:
  explicit CLIException(const std::string& message);
  virtual const char* what() const noexcept override;
private:
  std::string message;
};

// Handles missing, unreadable, or otherwise inaccessible files
class FileException : public std::exception {
public:
  explicit FileException(const std::string& message);
  virtual const char* what() const noexcept;
private:
  std::string message;
};
#endif