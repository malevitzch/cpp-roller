cmake_minimum_required(VERSION 3.28)

project(cpp-roller LANGUAGES CXX VERSION 1.3.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include(CTest)
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
