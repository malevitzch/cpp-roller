#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP
#include "roller.hpp"
#ifdef _WIN32
  using arg_t = wchar_t*;
#else
  using arg_t = char*;
#endif
using args_t = arg_t*;
RollerConfig parse_args(int argc, args_t argv);
#endif