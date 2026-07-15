#include "common.hpp"

std::size_t hash_combine(std::size_t h1, std::size_t h2) {
  return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

CLIException::CLIException(const string_t& message) : message(message) {}
string_t CLIException::what() const noexcept {
  return message;
}

FileException::FileException(const string_t& message) : message(message) {}
string_t FileException::what() const noexcept {
  return message;
}