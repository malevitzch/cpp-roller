#ifndef CLEANER_HPP
#define CLEANER_HPP

#include <gtest/gtest.h>

#include <vector>
#include <filesystem>
#include "common.hpp"

class Cleaner {
private:
  std::vector<std::filesystem::path> files;
protected:
  void registerFile(const string_t& file) {
    files.push_back(path_from_utf8(file));
  }
  void cleanup() {
    for(const auto& file : files)
      std::filesystem::remove(file);
    files.clear();
  }
};

class TestCleaner : public testing::Test, protected Cleaner {
protected:
  void TearDown() override {
    cleanup();
  }
};

#endif
