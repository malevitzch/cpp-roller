#ifndef ROLLER_HPP
#define ROLLER_HPP

#include <vector>
#include <filesystem>

void roll(std::vector<std::filesystem::path> files, std::filesystem::path output_filename);

#endif
