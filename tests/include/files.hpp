#ifndef FILES_HPP
#define FILES_HPP
#include <filesystem>
#include <regex>

std::string stringify_file(std::filesystem::path file);
int count_occurences(std::filesystem::path file, std::regex regex);

#endif
