#ifndef FILES_HPP
#define FILES_HPP
#include <filesystem>
#include <regex>

void assert_file_exists(std::filesystem::path file);

std::string stringify_file(std::filesystem::path file);
int count_occurences(std::filesystem::path file, std::regex regex);
int count_occurences(std::filesystem::path file, std::string pattern);

#endif
