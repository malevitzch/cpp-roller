#include "common.hpp"



std::size_t hash_combine(std::size_t h1, std::size_t h2) {
  return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

UserException::UserException(const std::string& message) : message_(message) {}
const char* UserException::what() const noexcept {
  return message_.c_str();
}
