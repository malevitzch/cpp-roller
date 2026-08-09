#ifndef FILES_HPP
#define FILES_HPP
#include <filesystem>
#include "common.hpp"

void assert_file_exists(string_t file);

string_t stringify_file(string_t file);
int count_occurences(string_t file, roller_regex_t regex);
int count_occurences(string_t file, string_t pattern);

#endif
