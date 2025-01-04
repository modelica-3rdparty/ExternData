cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
  if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

project(ExternData C)

file(GENERATE OUTPUT .gitignore CONTENT "*")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(BuildProjects/CMake/Modelica_platform.cmake)

set(ED_RESOURCES_DIR "${CMAKE_SOURCE_DIR}")

get_modelica_platform_name(ED_PLATFORM_PATH_SUFFIX)

include(BuildProjects/CMake/options.cmake)

if(ED_BUILD_TESTING)
  enable_testing()
  include(BuildProjects/CMake/test.cmake)
endif()
