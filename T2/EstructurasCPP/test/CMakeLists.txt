add_executable(
  PuntoTests
  testPuntoClassFixtures.cpp
)

#https://google.github.io/googletest/quickstart-cmake.html
target_link_libraries(
    PuntoTests
    gtest_main
    punto
)

include(GoogleTest)
gtest_discover_tests(PuntoTests)