#include "files.hpp"
#include <fstream>

#include <gtest/gtest.h>

void assert_file_exists(std::filesystem::path file) {
    ASSERT_TRUE(std::filesystem::exists(file))
        << "File does not exist: " << file;

    ASSERT_TRUE(std::filesystem::is_regular_file(file))
        << "Path is not a regular file: " << file;
}

std::string stringify_file(std::filesystem::path file) {
  if(!std::filesystem::exists(file)) {
    std::string msg = "Failed to load file \"" + file.string() + "\"";
    throw std::filesystem::filesystem_error(msg, std::make_error_code(std::errc::no_such_file_or_directory));
  }

  std::ifstream in(file);
  std::stringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

int count_occurences(std::filesystem::path file, std::regex regex) {
  std::string contents = stringify_file(file);

  auto begin = std::sregex_iterator(contents.begin(), contents.end(), regex);
  auto end = std::sregex_iterator();

  return std::distance(begin, end);
}

int count_occurences(std::filesystem::path file, std::string pattern) {
  return count_occurences(file, std::regex(pattern));
}
