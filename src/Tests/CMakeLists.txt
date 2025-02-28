file(GLOB TEST_SOURCES *.cpp)  # Find all test files

add_executable(my_tests ${TEST_SOURCES})  # Create test executable

target_include_directories(my_tests PRIVATE ${PROJECT_SOURCE_DIR}/src/YAC ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include)
target_link_libraries(my_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME MyTests COMMAND my_tests)  # Register tests with CTest
