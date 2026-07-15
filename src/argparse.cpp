#include "argparse.hpp"
#include "common.hpp"
#include <format>
#include <span>

#ifdef _WIN32

enum class TokType {
  ShortOpt,
  LongOpt,
  Other,
};

struct Token {
  TokType type;
  string_t value;
};

Token parse_tok(const arg_t tok) {
  Token res;
  int len = STRLEN(tok);
  if(tok[0] == STR('-')) {
    if(len >= 2 && tok[1] == STR('-')) {
      res.value = tok + 2;
      res.type = TokType::LongOpt;
    } else {
      res.value = tok + 1;
      res.type = TokType::ShortOpt;
    }
  } else {
    res.value = tok;
    res.type = TokType::Other;
  }
  return res;
}

struct OptArg {
  string_t longname;
  string_t shortname;
  size_t argcount;
};

RollerConfig parse_args(int argc, args_t argv) {
  RollerConfig config;
  std::span<arg_t> args(argv, argc);
  auto it = args.begin() + 1;

  std::vector<OptArg> optargs = {
    {STR("help"),    STR("h"), 0},
    {STR("version"), STR("v"), 0},
    {STR("output"),  STR("o"), 1},
    {STR("include"), STR("I"), 1},
  };
  std::map<string_t, OptArg> optmap;
  for(const auto& opt : optargs) {
    optmap[opt.longname] = opt;
    optmap[opt.shortname] = opt;
  }

  while(it != args.end()) {
    Token tok = parse_tok(*it);
    if(tok.type == TokType::LongOpt || tok.type == TokType::ShortOpt) {
      if(!optmap.contains(tok.value)) {
        throw CLIException(std::format(STR("Unknown option \"{}\""), tok.value));
      }
      auto opt = optmap[tok.value];
      size_t rem = std::distance(it, args.end()) - 1;
      if(rem < opt.argcount) {
        throw CLIException(std::format(STR("Option \"{}\" requires {} arguments"), tok.value, opt.argcount));
      }
      std::vector<string_t> optargs(it + 1, it + 1 + opt.argcount);
      if(opt.longname == STR("help")) {
        config.flag("help");
      } else if(opt.longname == STR("version")) {
        config.flag("version");
      } else if(opt.longname == STR("output")) {
        config.name(optargs[0]);
      } else if(opt.longname == STR("include")) {
        config.add_include_directories(optargs[0]);
      }
      it += opt.argcount;
    } else {
      config.add_source(tok.value);
    }
    it++;
  }
  return config;
}
#else
#include <getopt.h>
RollerConfig parse_args(int argc, char** argv) {
  static option long_opts[] = {
    {"help",    no_argument,       nullptr, 'h'},
    {"version", no_argument,       nullptr, 'v'},
    {"output",  required_argument, nullptr, 'o'},
    {"include", required_argument, nullptr, 'I'},
    {nullptr,   0,                 nullptr, 0},
  };

  RollerConfig config;
  int opt;
  while ((opt = getopt_long(argc, argv, ":hvo:I:", long_opts, nullptr)) != -1) {
    switch (opt) {
      case 'h':
        config.flag("help");
        break;
      case 'v':
        config.flag("version");
        break;
      case 'o':
        config.name(optarg);
        break;
      case 'I':
        config.add_include_directories(optarg);
        break;
      case ':': {
        throw CLIException(std::format(STR("The option \"{}\" requires an argument"), char(optopt)));
      }
      case '?':
        throw CLIException(std::format(STR("Unknown option \"{}\""), char(optopt)));
      default:
        throw CLIException(std::format(STR("Unknown error while parsing option \"{}\""), char(optopt)));
    }
  }
  for(int i = optind; i < argc; i++) {
    config.add_source(argv[i]);
  }
  return config;
}

#endif