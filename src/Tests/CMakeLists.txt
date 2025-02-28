file(GLOB TEST_SOURCES *.cpp)  # Find all test files

# Link the test executable with the main project sources
add_executable(YacTests ${TEST_SOURCES} ${SOURCES})

target_include_directories(YacTests PRIVATE ${PROJECT_SOURCE_DIR}/src/YAC ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include)
target_link_libraries(YacTests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME YacTests COMMAND YacTests)  # Register tests with CTest