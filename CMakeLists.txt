cmake_minimum_required(VERSION 3.28)

project(cpp-roller LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(ABSL_ENABLE_INSTALL ON CACHE BOOL "" FORCE)
set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20260107.0
)
FetchContent_MakeAvailable(absl)

FetchContent_Declare(
  re2
  GIT_REPOSITORY https://github.com/google/re2.git
  GIT_TAG 2025-11-05
)
FetchContent_MakeAvailable(re2)


add_subdirectory(src)
