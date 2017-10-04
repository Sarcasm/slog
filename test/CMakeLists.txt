include(GoogleTest)

find_package(GTest REQUIRED)

add_executable(tst-logger tst-logger.cpp)
target_link_libraries(tst-logger slog::libslog GTest::GTest GTest::Main)
gtest_add_tests(TARGET tst-logger)
