//
// Created by satya on 28-02-2025.
//
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>

#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"
#include "YAC/CodeAnalysis/Compilation.h"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"
#include "YAC/CodeAnalysis/Syntax/Parser.h"

// Test fixture for evaluation tests
struct EvaluationTests : public ::testing::TestWithParam<std::pair<std::string, std::any>> {
};

// Test case for evaluating syntax expressions
TEST_P(EvaluationTests, SyntaxFact_GetText_RoundTrips) {
    const auto& [text, expectedValue] = GetParam();

    auto parser = new Parser(text);
    auto syntaxTree = parser->parse();
    auto compilation = new Compilation(*syntaxTree);
    std::unordered_map<VariableSymbol, std::any> variables;
    const auto result = compilation->evaluate(variables);

    // Ensure there are no diagnostics (errors) in the evaluation result
    ASSERT_TRUE(result.diagnostics().empty());

    // Verify the expected value
    if (expectedValue.type() == typeid(int)) {
        EXPECT_EQ(std::any_cast<int>(result.value()), std::any_cast<int>(expectedValue));
    } else if (expectedValue.type() == typeid(bool)) {
        EXPECT_EQ(std::any_cast<bool>(result.value()), std::any_cast<bool>(expectedValue));
    } else {
        FAIL() << "Unexpected type in test case";
    }
}

// Test data
std::vector<std::pair<std::string, std::any>> GetEvaluationTestCases() {
    return {
        {"1", 1},
        {"+1", 1},
        {"-1", -1},
        {"14 + 12", 26},
        {"12 - 3", 9},
        {"4 * 2", 8},
        {"9 / 3", 3},
        {"(10)", 10},
        {"12 == 3", false},
        {"3 == 3", true},
        {"12 != 3", true},
        {"3 != 3", false},
        {"false == false", true},
        {"true == false", false},
        {"false != false", false},
        {"true != false", true},
        {"true", true},
        {"false", false},
        {"!true", false},
        {"!false", true},
        {"(a = 10) * a", 100}
    };
}

// Instantiate the test cases
INSTANTIATE_TEST_SUITE_P(
    ParserTests,
    EvaluationTests,
    ::testing::ValuesIn(GetEvaluationTestCases())
);
