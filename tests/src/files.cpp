#include "files.hpp"
#include "common.hpp"

#include <gtest/gtest.h>

void assert_file_exists(std::filesystem::path file) {
    ASSERT_TRUE(std::filesystem::exists(file))
        << "File does not exist: " << file;

    ASSERT_TRUE(std::filesystem::is_regular_file(file))
        << "Path is not a regular file: " << file;
}

string_t stringify_file(std::filesystem::path file) {
  if(!std::filesystem::exists(file)) {
    throw std::filesystem::filesystem_error(
      "Failed to load file",
      file,
      std::make_error_code(std::errc::no_such_file_or_directory)
    );
  }

  ifstream_t in(file);
  std::basic_stringstream<typename string_t::value_type, typename string_t::traits_type, typename string_t::allocator_type> buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

int count_occurences(std::filesystem::path file, roller_regex_t regex) {
  string_t contents = stringify_file(file);

  auto begin = std::regex_iterator<typename string_t::const_iterator>(
    contents.begin(), contents.end(), regex);
  auto end = std::regex_iterator<typename string_t::const_iterator>();

  return std::distance(begin, end);
}

int count_occurences(std::filesystem::path file, string_t pattern) {
  return count_occurences(file, roller_regex_t(pattern));
}
