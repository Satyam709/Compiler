file(GLOB_RECURSE TEST_SOURCES *.cpp)  # Find all .cpp files recursively

# Link the test executable with the found sources
add_executable(YacTests ${TEST_SOURCES}
        CodeAnalysis/Syntax/AssertingEnumerator.cpp
        CodeAnalysis/Syntax/AssertingEnumerator.cpp
        CodeAnalysis/Syntax/AssertingEnumerator.h
        CodeAnalysis/Syntax/ParserTests.cpp
        CodeAnalysis/EvaluationTests.cpp)


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