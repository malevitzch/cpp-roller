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
  std::string value;
};

Token parse_tok(const char* tok) {
  Token res;
  int len = strlen(tok);
  if(tok[0] == '-') {
    if(len >= 2 && tok[1] == '-') {
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
  std::string longname;
  std::string shortname;
  size_t argcount;
};

RollerConfig parse_args(int argc, char** argv) {
  RollerConfig config;
  std::span<char*> args(argv, argc);
  auto it = args.begin() + 1;

  std::vector<OptArg> optargs = {
    {"help",    "h", 0},
    {"version", "v", 0},
    {"output",  "o", 1},
    {"include", "I", 1},
  };
  std::map<std::string, OptArg> optmap;
  for(const auto& opt : optargs) {
    optmap[opt.longname] = opt;
    optmap[opt.shortname] = opt;
  }

  while(it != args.end()) {
    Token tok = parse_tok(*it);
    if(tok.type == TokType::LongOpt || tok.type == TokType::ShortOpt) {
      if(!optmap.contains(tok.value)) {
        throw CLIException(std::format("Unknown option \"{}\"", tok.value));
      }
      auto opt = optmap[tok.value];
      size_t rem = std::distance(it, args.end()) - 1;
      if(rem < opt.argcount) {
        throw CLIException(std::format("Option \"{}\" requires {} arguments", tok.value, opt.argcount));
      }
      std::vector<std::string> optargs(it + 1, it + 1 + opt.argcount);
      if(opt.longname == "help") {
        config.flag("help");
      } else if(opt.longname == "version") {
        config.flag("version");
      } else if(opt.longname == "output") {
        config.name(optargs[0]);
      } else if(opt.longname == "include") {
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
        throw CLIException(std::format("The option \"{}\" requires an argument", char(optopt)));
      }
      case '?':
        throw CLIException(std::format("Unknown option \"{}\"", char(optopt)));
      default:
        throw CLIException(std::format("Unknown error while parsing option \"{}\"", char(optopt)));
    }
  }
  for(int i = optind; i < argc; i++) {
    config.add_source(argv[i]);
  }
  return config;
}

#endif