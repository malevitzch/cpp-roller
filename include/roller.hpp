#ifndef ROLLER_HPP
#define ROLLER_HPP
#include "common.hpp"

#include <set>
#include <map>
#include <filesystem>
#include <vector>

enum class RollResult : int {
  Success = 0,
  UserError = 1, // Bad CLI usage
  FilesystemFailure = 2, // Missing/unreadable file
  UnexplainedFailure = 3, // Anything else
};

class RollerConfig {
  private:
    string_t _output_name = STR("rolled.out");
    std::set<std::filesystem::path> _sources;
    std::map<std::string, bool> _flags;
    std::vector<std::filesystem::path> _include_paths;
  public:
    RollerConfig() = default;
    RollerConfig& name(string_t new_name);
    RollerConfig& add_source(string_t source);
    RollerConfig& add_include_directories(string_t paths);
    RollerConfig& flag(std::string name, bool value);
    RollerConfig& flag(std::string name);

    const std::set<std::filesystem::path>& get_sources();
    string_t get_output_name();
    bool get_flag(std::string name);
    const std::vector<std::filesystem::path>& get_include_dirs();
};

RollResult roll(RollerConfig& config);

#endif
