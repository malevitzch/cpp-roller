#ifndef ROLLER_HPP
#define ROLLER_HPP


#include <set>
#include <filesystem>

class RollerConfig {
  private:
    std::string _output_name = "a.out";
    std::set<std::filesystem::path> _sources;
    bool _version = false;
  public:
    RollerConfig() = default;
    RollerConfig& name(std::string new_name);
    RollerConfig& add_source(std::string source);
    RollerConfig& version_flag();

    const std::set<std::filesystem::path>& get_sources();
    std::string get_output_name();
    bool get_version_flag();
};

void roll(RollerConfig& config);

#endif
