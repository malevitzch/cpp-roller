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
    assert_file_exists(STR("diamond/parent.cpp"));
    assert_file_exists(STR("diamond/left.cpp"));
    assert_file_exists(STR("diamond/right.cpp"));
    assert_file_exists(STR("diamond/common.cpp"));
    // TODO: make roll perhaps allow output to a buffer rather than a file if a certain option is set
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("diamond/parent.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("COMMON_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("LEFT_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("RIGHT_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("PARENT_INCLUDED")), 1);
  }
  TEST_F(TestCleaner, Relative) {
    assert_file_exists(STR("relative/parent.cpp"));
    assert_file_exists(STR("relative/children/direct.cpp"));
    assert_file_exists(STR("relative/children/transitive.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("relative/parent.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("PARENT_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("DIRECT_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("TRANSITIVE_INCLUDED")), 1);
  }
  /*
   * When multiple roots are given,
   * they should still be rolled together.
   */
  TEST_F(TestCleaner, MultiSource) {
    assert_file_exists(STR("multi-source/common.cpp"));
    assert_file_exists(STR("multi-source/sourceA.cpp"));
    assert_file_exists(STR("multi-source/sourceB.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("multi-source/sourceA.cpp"))
      .add_source(STR("multi-source/sourceB.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("SOURCE_A_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("SOURCE_B_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("COMMON_INCLUDED")), 1);
  }
  /*
   * Angle includes such as <iostream> should be collected
   * and added at the beginning of the whole file.
   */
  TEST_F(TestCleaner, TestAngleIncludeDedup) {
    assert_file_exists(STR("angle-include-dedup/left.cpp"));
    assert_file_exists(STR("angle-include-dedup/right.cpp"));
    assert_file_exists(STR("angle-include-dedup/parent.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("angle-include-dedup/parent.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("<iostream>")), 1);
  }
  /*
   * Completely identical files should only be included once.
   */
  TEST_F(TestCleaner, TestDuplicateFile) {
    assert_file_exists(STR("duplicate-file/parent.cpp"));
    assert_file_exists(STR("duplicate-file/child.cpp"));
    assert_file_exists(STR("duplicate-file/child_copy.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("duplicate-file/parent.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("CHILD_INCLUDED")), 1);
  }
  /*
   * Filenames containing unicode characters should be recognized.
   */
  TEST_F(TestCleaner, TestUnicode) {
    assert_file_exists(STR("unicode/parent.cpp"));
    assert_file_exists(STR("unicode/中文.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("unicode/parent.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("CHINESE_INCLUDED")), 1);
  }
  /*
   * Colon
   */
  TEST_F(TestCleaner, TestIncludeDirectories) {
    assert_file_exists(STR("include-directories/parent.cpp"));
    assert_file_exists(STR("include-directories/libA/childA.cpp"));
    assert_file_exists(STR("include-directories/libB/childB.cpp"));
    registerFile(STR("rolled.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("include-directories/parent.cpp"))
      .add_include_directories(std::format(STR("include-directories/libA/{}include-directories/libB/"), MULTIPATH_SEP))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::Success);
    assert_file_exists(STR("rolled.cpp"));
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("CHILD_A_INCLUDED")), 1);
    ASSERT_EQ(count_occurences(STR("rolled.cpp"), STR("CHILD_B_INCLUDED")), 1);
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
      .add_source(STR("does_not_exist.cpp"))
      .name(STR("rolled.cpp"));
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }

  TEST(TestExitCode, IncludingNonexistentFileIsFilesystemFailure) {
    assert_file_exists(STR("nonexistent-include.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("nonexistent-include.cpp"));
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }

  TEST(TestExitCode, NonexistentIncludeDirectoryIsFilesystemFailure) {
    assert_file_exists(STR("empty.cpp"));
    RollerConfig conf = RollerConfig()
      .add_source(STR("empty.cpp"))
      .add_include_directories(STR("this_directory_does_not_exist"));
    EXPECT_EQ(roll(conf), RollResult::FilesystemFailure);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
