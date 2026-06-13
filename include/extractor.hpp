#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#ifndef DEBUG
#define DEBUG false
#endif

#include <string>
#include <vector>
#include <set>
std::string extract_blob(std::string filename);
std::vector<std::string> extract_includes(std::string filename, std::set<std::string>& angle_include_set);

#endif
