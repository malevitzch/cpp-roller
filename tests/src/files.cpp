#include "files.hpp"

#include <fstream>

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
