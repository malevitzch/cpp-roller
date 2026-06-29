#include <gtest/gtest.h>
#include <filesystem>

#include "files.hpp"
#include "roller.hpp"

#include "fixtures/cleaner.hpp"

namespace {
  /*
   * A dependency of two children should only be included once.
   */
  TEST_F(TestCleaner, Diamond) {
    assert_file_exists("diamond/parent.cpp");
    assert_file_exists("diamond/left.cpp");
    assert_file_exists("diamond/right.cpp");
    assert_file_exists("diamond/common.cpp");
    // TODO: make roll perhaps allow output to a buffer rather than a file if a certain option is set
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("diamond/parent.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "COMMON_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "LEFT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "RIGHT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "PARENT_INCLUDED"), 1);
  }
  TEST_F(TestCleaner, Relative) {
    assert_file_exists("relative/parent.cpp");
    assert_file_exists("relative/children/direct.cpp");
    assert_file_exists("relative/children/transitive.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("relative/parent.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "PARENT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "DIRECT_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "TRANSITIVE_INCLUDED"), 1);
  }
  /*
   * When multiple roots are given,
   * they should still be rolled together.
   */
  TEST_F(TestCleaner, MultiSource) {
    assert_file_exists("multi-source/common.cpp");
    assert_file_exists("multi-source/sourceA.cpp");
    assert_file_exists("multi-source/sourceB.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("multi-source/sourceA.cpp")
      .add_source("multi-source/sourceB.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "SOURCE_A_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "SOURCE_B_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "COMMON_INCLUDED"), 1);
  }
  /*
   * Angle includes such as <iostream> should be collected
   * and added at the beginning of the whole file.
   */
  TEST_F(TestCleaner, TestAngleIncludeDedup) {
    assert_file_exists("angle-include-dedup/left.cpp");
    assert_file_exists("angle-include-dedup/right.cpp");
    assert_file_exists("angle-include-dedup/parent.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("angle-include-dedup/parent.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "<iostream>"), 1);
  }
  /*
   * Completely identical files should only be included once.
   */
  TEST_F(TestCleaner, TestDuplicateFile) {
    assert_file_exists("duplicate-file/parent.cpp");
    assert_file_exists("duplicate-file/child.cpp");
    assert_file_exists("duplicate-file/child_copy.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("duplicate-file/parent.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "CHILD_INCLUDED"), 1);      
  }
  TEST_F(TestCleaner, TestUnicode) {
    assert_file_exists("unicode/parent.cpp");
    assert_file_exists("unicode/中文.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("unicode/parent.cpp")
      .name("rolled.cpp");
    roll(conf);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "CHINESE_INCLUDED"), 1);      
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
