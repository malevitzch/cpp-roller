#ifndef COMMON_HPP
#define COMMON_HPP

#ifndef DEBUG
#define DEBUG false
#endif

#ifndef ROLLER_VERSION
#define ROLLER_VERSION "???"
#endif


#include <cstddef>
std::size_t hash_combine(std::size_t h1, std::size_t h2);

#endif