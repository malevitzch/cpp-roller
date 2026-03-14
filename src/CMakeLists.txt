set(ROLL_SOURCES
    main.cpp)

set(ROLLIB_SOURCES
    extractor.cpp
    grapher.cpp
    roller.cpp)

add_library(rollib ${ROLLIB_SOURCES})
target_include_directories(rollib PRIVATE ../include/)

add_executable(cpproll ${ROLL_SOURCES})
target_link_libraries(cpproll PRIVATE rollib)
target_include_directories(cpproll PRIVATE ../include/)
target_compile_definitions(cpproll PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_compile_definitions(rollib PRIVATE
  ROLLER_VERSION="${PROJECT_VERSION}"
)

target_compile_definitions(rollib PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(cpproll PRIVATE $<$<CONFIG:Debug>:DEBUG>)
