//
// Created by satya on 15-04-2025.
//
// SourceTextTests.cpp
#include <gtest/gtest.h>
#include <string>
#include "YAC/CodeAnalysis/Text/SourceText.h"

struct SourceTextTests : public ::testing::TestWithParam<std::pair<std::string, int>> {
};

TEST_P(SourceTextTests, SourceTextLineCount) {
    const auto &[text, expectedValue] = GetParam();
    const auto sourceText = SourceText(text);
    const unsigned int lineCount = sourceText.GetLines().size();
    ASSERT_EQ(lineCount, expectedValue)
        << "Line count mismatch for input: \"" << text << "\"\n"
        << "Expected: " << static_cast<int>(expectedValue)
        << ", but got: " << static_cast<int>(lineCount);
}

std::vector<std::pair<std::string, int>> GetLineCountTestCases() {
    return {
            {".", 1},
            {".\r\n", 2},
            {".\r\n\r\n", 3}
    };
}

INSTANTIATE_TEST_SUITE_P(
    SourceText,
    SourceTextTests,
    ::testing::ValuesIn(GetLineCountTestCases())
);
