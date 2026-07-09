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

std::size_t hash_combine(std::size_t h1, std::size_t h2);

class UserException : public std::exception {
public:
  explicit UserException(const std::string& message);
  virtual const char* what() const noexcept override;
private:
  std::string message_;
};

#endif