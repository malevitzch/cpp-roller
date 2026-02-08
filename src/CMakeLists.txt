set(ROLL_SOURCES
  main.cpp
  extractor.cpp
  grapher.cpp
)

add_executable(cpproll ${ROLL_SOURCES})

target_include_directories(cpproll PRIVATE ../include/)
