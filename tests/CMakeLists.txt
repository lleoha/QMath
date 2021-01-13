find_package(GTest REQUIRED)

add_executable(QFloatTests multiply.cpp)
target_link_libraries(QFloatTests GTest::GTest GTest::Main)

gtest_discover_tests(QFloatTests)
