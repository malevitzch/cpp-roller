#ifndef FILES_HPP
#define FILES_HPP
#include <filesystem>
#include "common.hpp"

void assert_file_exists(std::filesystem::path file);

string_t stringify_file(std::filesystem::path file);
int count_occurences(std::filesystem::path file, regex_t regex);
int count_occurences(std::filesystem::path file, string_t pattern);

#endif
