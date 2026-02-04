add_executable(cpproll main.cpp extractor.cpp)
target_include_directories(cpproll PRIVATE ../include/)
target_include_directories(cpproll PRIVATE ${jpcre2_SOURCE_DIR}/include)
target_link_libraries(cpproll pcre2-8-static)
