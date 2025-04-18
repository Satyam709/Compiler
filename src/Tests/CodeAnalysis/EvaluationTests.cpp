//
// Created by satya on 28-02-2025.
//
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>

#include "AnnotatedText.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"
#include "YAC/CodeAnalysis/Compilation.h"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"
#include "YAC/CodeAnalysis/Syntax/Parser.h"

// Test fixture for evaluation tests
struct EvaluationTests : public ::testing::TestWithParam<std::pair<std::string, std::any> > {
};

static void AssertValue(const std::string &text, const std::any &expectedValue) {
    const auto syntaxTree = new SyntaxTree(text);
    const auto compilation = new Compilation(*syntaxTree);
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

static void AssertDiagnostics(const std::string &text, const std::string &diagnosticText) {
    const auto annotatedText = AnnotatedText::Parse(text);
    const auto syntaxTree = SyntaxTree::parse(SourceText::From(annotatedText.getText()));
    const auto compilation = Compilation(*syntaxTree);
    std::unordered_map<VariableSymbol, std::any> variables;
    const auto result = compilation.evaluate(variables);

    const auto expectedDiagnostics = AnnotatedText::UnindentLines(diagnosticText);

    if (annotatedText.getSpans().size() != expectedDiagnostics.size()) {
        throw std::runtime_error("ERROR: Must mark as many spans as there are expected diagnostics");
    }

    ASSERT_EQ(expectedDiagnostics.size(), result.diagnostics().size());

    for (size_t i = 0; i < expectedDiagnostics.size(); i++) {
        const auto &expectedMessage = expectedDiagnostics[i];
        const auto &actualMessage = result.diagnostics()[i].getMessage();
        EXPECT_EQ(expectedMessage, actualMessage);
        const auto &expectedSpan = annotatedText.getSpans()[i];
        const auto &actualSpan = result.diagnostics()[i].getSpan();

        EXPECT_EQ(expectedSpan.Start(), actualSpan.Start());
        EXPECT_EQ(expectedSpan.Length(), actualSpan.Length());
    }
}


// Test case for evaluating syntax expressions
TEST_P(EvaluationTests, SyntaxFact_GetText_RoundTrips) {
    const auto &[text, expectedValue] = GetParam();
    AssertValue(text, expectedValue);
}

// Test data
std::vector<std::pair<std::string, std::any> > GetEvaluationTestCases() {
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
        {"3 < 4", true},
        {"5 < 4", false},
        {"4 <= 4", true},
        {"4 <= 5", true},
        {"5 <= 4", false},
        {"4 > 3", true},
        {"4 > 5", false},
        {"4 >= 4", true},
        {"5 >= 4", true},
        {"4 >= 5", false},
        {"{ var a = 0 (a = 10) * a }", 100},
        {"{ var a = 0 if a == 0 a = 10 a }", 10},
        {"{ var a = 0 if a == 4 a = 10 a }", 0},
        {"{ var a = 0 if a == 0 a = 10 else a = 5 a }", 10},
        {"{ var a = 0 if a == 4 a = 10 else a = 5 a }", 5},
        {"{ var i = 10 var result = 0 while i > 0 { result = result + i i = i - 1} result }", 55},
        {"{ var result = 0 for i = 1 to 10 { result = result + i } result }", 55},
    };
}

TEST_F(EvaluationTests, Evaluator_NameExpression_Reports_NoErrorForInsertedToken) {
    const std::string text = "[]";

    const std::string diagnostics = R"(
        Unexpected token <EndOfFileToken>, expected <IdentifierToken>.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_BlockStatement_NoInfiniteLoop) {
    const std::string text = R"(
        {
            [)][]

    )";

    const std::string diagnostics = R"(
        Unexpected token <CloseParenthesisToken>, expected <IdentifierToken>.
        Unexpected token <EndOfFileToken>, expected <CloseBraceToken>.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_ForStatement_Reports_CannotConvert_LowerBound) {
    const std::string text = R"(
        {
            var result = 0
            for i = [false] to 10
                result = result + i
        }
    )";

    const std::string diagnostics = R"(
        Cannot convert type 'bool' to 'int'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_ForStatement_Reports_CannotConvert_UpperBound) {
    const std::string text = R"(
        {
            var result = 0
            for i = 1 to [true]
                result = result + i
        }
    )";

    const std::string diagnostics = R"(
        Cannot convert type 'bool' to 'int'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_WhileStatement_Reports_CannotConvert) {
    const std::string text = R"(
        {
            var x = 0
            while [10]
                x = 10
        }
    )";

    const std::string diagnostics = R"(
        Cannot convert type 'int' to 'bool'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_IfStatement_Reports_CannotConvert) {
    const std::string text = R"(
        {
            var x = 0
            if [10]
                x = 10
        }
    )";

    const std::string diagnostics = R"(
        Cannot convert type 'int' to 'bool'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_VariableDeclaration_Reports_Redeclaration) {
    const std::string text = R"(
        {
            var x = 10
            var y = 100
            {
                var x = 10
            }
            var [x] = 5
        }
    )";

    const std::string diagnostics = R"(
        Variable 'x' is already declared.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Name_Reports_Undefined) {
    const std::string text = "[x] * 10";

    const std::string diagnostics = R"(
        Variable 'x' does not exist.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Assigned_Reports_Undefined) {
    const std::string text = "[x] = 10";

    const std::string diagnostics = R"(
        Variable 'x' does not exist.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Assigned_Reports_CannotAssign) {
    const std::string text = R"(
        {
            let x = 10
            x [=] 0
        }
    )";

    const std::string diagnostics = R"(
        Variable 'x' is read-only and cannot be assigned to.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Assigned_Reports_CannotConvert) {
    const std::string text = R"(
        {
            var x = 10
            x = [true]
        }
    )";

    const std::string diagnostics = R"(
        Cannot convert type 'bool' to 'int'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Unary_Reports_Undefined) {
    const std::string text = "[+]true";

    const std::string diagnostics = R"(
        Unary operator '+' is not defined for type 'bool'.
    )";

    AssertDiagnostics(text, diagnostics);
}

TEST_F(EvaluationTests, Evaluator_Binary_Reports_Undefined) {
    const std::string text = "10 [*] false";

    const std::string diagnostics = R"(
        Binary operator '*' is not defined for types 'int' and 'bool'.
    )";

    AssertDiagnostics(text, diagnostics);
}

// Instantiate the test cases
INSTANTIATE_TEST_SUITE_P(
    ParserTests,
    EvaluationTests,
    ::testing::ValuesIn(GetEvaluationTestCases())
);
