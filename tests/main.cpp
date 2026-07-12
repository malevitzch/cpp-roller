#include <gtest/gtest.h>
#include <filesystem>
#include <format>

#include "files.hpp"
#include "roller.hpp"
#include "common.hpp"

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
    EXPECT_EQ(roll(conf), RollResult::Success);
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
    EXPECT_EQ(roll(conf), RollResult::Success);
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
    EXPECT_EQ(roll(conf), RollResult::Success);
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
    EXPECT_EQ(roll(conf), RollResult::Success);
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
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "CHILD_INCLUDED"), 1);
  }
  /*
   * Filenames containing unicode characters should be recognized.
   */
  TEST_F(TestCleaner, TestUnicode) {
    assert_file_exists("unicode/parent.cpp");
    assert_file_exists("unicode/中文.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("unicode/parent.cpp")
      .name("rolled.cpp");
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "CHINESE_INCLUDED"), 1);
  }
  /*
   * Colon
   */
  TEST_F(TestCleaner, TestIncludeDirectories) {
    assert_file_exists("include-directories/parent.cpp");
    assert_file_exists("include-directories/libA/childA.cpp");
    assert_file_exists("include-directories/libB/childB.cpp");
    registerFile("rolled.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("include-directories/parent.cpp")
      .add_include_directories(std::format("include-directories/libA/{}include-directories/libB/", MULTIPATH_SEP))
      .name("rolled.cpp");
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists("rolled.cpp");
    ASSERT_EQ(count_occurences("rolled.cpp", "CHILD_A_INCLUDED"), 1);
    ASSERT_EQ(count_occurences("rolled.cpp", "CHILD_B_INCLUDED"), 1);
  }
  // Tests for proper "roll" return codes in various scenarios

  TEST(TestExitCode, NoSourcesIsUserError) {
    RollerConfig conf = RollerConfig();
    EXPECT_EQ(roll(conf), RollResult::UserError);
  }

  TEST(TestExitCode, VersionFlagSucceeds) {
    RollerConfig conf = RollerConfig().flag("version");
    EXPECT_EQ(roll(conf), RollResult::Success);
  }

  TEST(TestExitCode, HelpFlagSucceeds) {
    RollerConfig conf = RollerConfig().flag("help");
    EXPECT_EQ(roll(conf), RollResult::Success);
  }

  TEST(TestExitCode, MissingSourceIsFilesystemFailure) {
    RollerConfig conf = RollerConfig()
      .add_source("does_not_exist.cpp")
      .name("rolled.cpp");
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }

  TEST(TestExitCode, IncludingNonexistentFileIsFilesystemFailure) {
    assert_file_exists("nonexistent-include.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("nonexistent-include.cpp");
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }

  TEST(TestExitCode, NonexistentIncludeDirectoryIsFilesystemFailure) {
    assert_file_exists("empty.cpp");
    RollerConfig conf = RollerConfig()
      .add_source("empty.cpp")
      .add_include_directories("this_directory_does_not_exist");
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
