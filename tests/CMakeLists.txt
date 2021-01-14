find_package(GTest REQUIRED)

add_executable(QFloatTests multiply.cpp divide.cpp add.cpp subtract.cpp)
target_link_libraries(QFloatTests GTest::GTest GTest::Main)

gtest_discover_tests(QFloatTests)
