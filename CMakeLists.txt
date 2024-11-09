cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(ObservableProperty VERSION 0.0.1)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

add_library(observable_property INTERFACE)

target_include_directories(observable_property INTERFACE include)
target_include_directories(observable_property SYSTEM INTERFACE include/observable_property/detail)

add_library(wolfsound::observable_property ALIAS observable_property)

option(BUILD_TESTS "If set to ON, JUCE and GoogleTest will be downloaded to configure the test project" OFF)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
