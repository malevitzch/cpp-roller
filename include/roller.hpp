#ifndef ROLLER_HPP
#define ROLLER_HPP


#include <set>
#include <map>
#include <filesystem>

class RollerConfig {
  private:
    std::string _output_name = "a.out";
    std::set<std::filesystem::path> _sources;
    std::map<std::string, bool> _flags;
    bool _version = false;
  public:
    RollerConfig() = default;
    RollerConfig& name(std::string new_name);
    RollerConfig& add_source(std::string source);
    RollerConfig& flag(std::string name, bool value);
    RollerConfig& flag(std::string name);

    const std::set<std::filesystem::path>& get_sources();
    std::string get_output_name();
    bool get_flag(std::string name);
};

void roll(RollerConfig& config);

#endif
