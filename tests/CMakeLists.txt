project(ObservablePropertyTests)
enable_testing()

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs)
include(../cmake/cpm.cmake)

cpmaddpackage(
  NAME
  GOOGLETEST
  GITHUB_REPOSITORY
  google/googletest
  GIT_TAG
  v1.15.2
  VERSION
  1.15.2
  SOURCE_DIR
  ${LIB_DIR}/googletest
  OPTIONS
  "INSTALL_GTEST OFF"
  "gtest_force_shared_crt ON"
)

# Needed for LiveObservableProperty
cpmaddpackage(
  NAME
  JUCE
  GITHUB_REPOSITORY
  juce-framework/JUCE
  GIT_TAG
  8.0.3
  VERSION
  8.0.3
  SOURCE_DIR
  ${LIB_DIR}/juce
)

include(../cmake/CompilerWarnings.cmake)
include(../cmake/Util.cmake)

set(SOURCES src/ObservablePropertyTests.cpp src/juce/LiveObservablePropertyTests.cpp)
add_executable(ObservablePropertyTests ${SOURCES})

set_source_files_properties(${SOURCES} PROPERTIES COMPILE_OPTIONS "${PROJECT_WARNINGS_CXX}")

target_link_libraries(ObservablePropertyTests PRIVATE wolfsound::observable_property)
target_link_libraries_system(
  ObservablePropertyTests
  PRIVATE
  GTest::gtest_main
  juce::juce_events
)

target_compile_definitions(
  ObservablePropertyTests PUBLIC JUCE_MODAL_LOOPS_PERMITTED=1 JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0
)

include(GoogleTest)
gtest_discover_tests(ObservablePropertyTests)
