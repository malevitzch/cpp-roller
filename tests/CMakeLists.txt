include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

include_directories(include)


set(TEST_SOURCES
    main.cpp
    src/files.cpp)

add_executable(main_test ${TEST_SOURCES})
target_include_directories(main_test PRIVATE include/)
target_link_libraries(main_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(main_test)
