#include <gtest/gtest.h>
#include <filesystem>

#include "files.hpp"
#include "roller.hpp"

namespace {
  TEST(FuncTests, Diamond) {
    assert_file_exists("diamond/parent.cpp");
    // TODO: implement (also make 'roll' more general)
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
