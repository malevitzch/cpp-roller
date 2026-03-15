#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#ifndef DEBUG
#define DEBUG false
#endif

#include <string>
#include <vector>
std::string remove_quote_includes(std::string buffer);
std::string extract_blob(std::string filename);
std::vector<std::string> extract_includes(std::string filename);

#endif
