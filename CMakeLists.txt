cmake_minimum_required(VERSION 3.28)

project(cpp-roller LANGUAGES CXX VERSION 3.0.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include(CTest)
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)
if(BUILD_TESTING)
  set(CMAKE_CTEST_ARGUMENTS --output-on-failure)
  add_subdirectory(tests)
endif()
