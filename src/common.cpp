#include "common.hpp"

std::size_t hash_combine(std::size_t h1, std::size_t h2) {
  return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

CLIException::CLIException(const std::string& message) : message(message) {}
const char* CLIException::what() const noexcept {
  return message.c_str();
}

FileException::FileException(const std::string& message) : message(message) {}
const char* FileException::what() const noexcept {
  return message.c_str();
}