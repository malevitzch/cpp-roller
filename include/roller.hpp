#ifndef ROLLER_HPP
#define ROLLER_HPP

#include <set>
#include <map>
#include <filesystem>
#include <vector>

class RollerConfig {
  private:
    std::string _output_name = "a.out";
    std::set<std::filesystem::path> _sources;
    std::map<std::string, bool> _flags;
    std::vector<std::filesystem::path> _include_paths;
    bool _version = false;
  public:
    RollerConfig() = default;
    RollerConfig& name(std::string new_name);
    RollerConfig& add_source(std::string source);
    RollerConfig& add_include_directories(std::string paths);
    RollerConfig& flag(std::string name, bool value);
    RollerConfig& flag(std::string name);

    const std::set<std::filesystem::path>& get_sources();
    std::string get_output_name();
    bool get_flag(std::string name);
    const std::vector<std::filesystem::path>& get_include_dirs();
};

void roll(RollerConfig& config);

#endif
