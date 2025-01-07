project(${CMAKE_PROJECT_NAME} CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  GIT_SHALLOW TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set_target_properties(gtest gtest_main PROPERTIES FOLDER "Test/Third-party")

set(ED_TEST_DIR "${ED_RESOURCES_DIR}/Test")
if(EXISTS "${ED_TEST_DIR}")
  set(MSL_URL https://raw.githubusercontent.com/modelica/ModelicaStandardLibrary/master)
  if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/EDTestCommon")
    file(DOWNLOAD
      "${MSL_URL}/.CI/Test/Common.c"
      "${CMAKE_CURRENT_BINARY_DIR}/EDTestCommon/Common.c")
  endif()
  set(TEST_COMMON_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/EDTestCommon/Common.c"
    "${ED_RESOURCES_DIR}/C-Sources/modelica/ModelicaUtilities.h"
  )
  add_library(EDTestCommon STATIC ${TEST_COMMON_SOURCES})
  set_target_properties(EDTestCommon PROPERTIES FOLDER "Test/Third-party")
  target_include_directories(EDTestCommon PRIVATE ${ED_RESOURCES_DIR}/C-Sources/modelica)

  set(ED_TESTS
    Test_ED_CSV
    Test_ED_INI
    Test_ED_JSON
    Test_ED_MAT
    Test_ED_XLS
    Test_ED_XLSX
    Test_ED_XML
    Test_ED_XML2
  )
  foreach(TEST ${ED_TESTS})
    add_executable(${TEST}
      "${ED_TEST_DIR}/${TEST}.cc"
      "${ED_TEST_DIR}/Constants.h"
    )
    target_compile_features(${TEST} PRIVATE cxx_std_17)
    set_target_properties(${TEST} PROPERTIES FOLDER "Test" CXX_EXTENSIONS OFF)
    set(ED_ALL_LIBS
      ED_CSVFile
      ED_INIFile
      ED_JSONFile
      ED_MATFile
      ED_XLSFile
      ED_XLSXFile
      ED_XML2File
      ED_XMLFile
      EDTestCommon
      bsxml-json
      expat
      hdf5
      minizip
      parson
      xlsreader
      xml2
      zlib
      bcrypt
      gtest
    )
    if(UNIX)
      list(APPEND ED_ALL_LIBS m)
    endif()
    target_link_libraries(${TEST} PRIVATE ${ED_ALL_LIBS})

    target_link_directories(${TEST} PRIVATE ${ED_RESOURCES_DIR}/Library/${ED_PLATFORM_PATH_SUFFIX})
    add_test(
      NAME ${TEST}
      COMMAND ${TEST}
      WORKING_DIRECTORY "${ED_TEST_DIR}"
    )
  endforeach()
  configure_file(
    ${CMAKE_SOURCE_DIR}/BuildProjects/CMake/ExternData.gta.runsettings.in
    ${CMAKE_BINARY_DIR}/ExternData.gta.runsettings
    @ONLY
  )
else()
  message(WARNING
    " Testsuite not found in \"${ED_TEST_DIR}\"."
    " Set BUILD_TESTING to OFF to silence this warning."
  )
endif()
