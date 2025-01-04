# Option to enable testsuite
option(ED_BUILD_TESTING "Build testing" ON)

set(BUILD_TESTING OFF)
if(ED_BUILD_TESTING)
  set(BUILD_TESTING ON)
endif()
