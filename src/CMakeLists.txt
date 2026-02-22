set(ROLL_SOURCES
  main.cpp
  extractor.cpp
  grapher.cpp
  roller.cpp
)

add_executable(cpproll ${ROLL_SOURCES})

target_include_directories(cpproll PRIVATE ../include/)
target_compile_definitions(cpproll PRIVATE
  ROLLER_VERSION="${PROJECT_VERSION}"
)
