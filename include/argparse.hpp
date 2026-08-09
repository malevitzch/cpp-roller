#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP
#include "roller.hpp"

using arg_t = char*;
using args_t = arg_t*;

RollerConfig parse_args(int argc, args_t argv);
#endif
