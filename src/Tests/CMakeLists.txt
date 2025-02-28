file(GLOB TEST_SOURCES *.cpp)  # Find all test files

# Link the test executable with the main project sources
add_executable(YacTests ${TEST_SOURCES})

# Link test executable with Google Test and CompilerLib
target_link_libraries(YacTests
        PRIVATE
        CompilerLib
        gtest
        gtest_main
        gmock
)

# Add tests
add_test(NAME YacTests COMMAND YacTests)  # Register tests with CTest