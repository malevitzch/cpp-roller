#include <gtest/gtest.h>
#include <filesystem>

#include "files.hpp"
#include "roller.hpp"

#include "fixtures/cleaner.hpp"

namespace {
  TEST_F(TestCleaner, Diamond) {
    assert_file_exists("diamond/parent.cpp");
    assert_file_exists("diamond/left.cpp");
    assert_file_exists("diamond/right.cpp");
    assert_file_exists("diamond/common.cpp");
    // TODO: make roll perhaps allow output to a buffer rather than a file if a certain option is set
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig().add_source("diamond/parent.cpp").name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "COMMON_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "LEFT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "RIGHT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "PARENT_INCLUDED"), 1);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
