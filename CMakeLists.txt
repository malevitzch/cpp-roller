cmake_minimum_required(VERSION 3.28)

project(cpp-roller LANGUAGES CXX VERSION 4.2.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(CTest)
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)
if(BUILD_TESTING)
  set(CMAKE_CTEST_ARGUMENTS --output-on-failure)
  add_subdirectory(tests)
endif()
