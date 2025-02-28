//
// Created by satya on 28-02-2025.
//


#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include <string>

#include "YAC/CodeAnalysis/Syntax/Parser.h"
#include "YAC/CodeAnalysis/Syntax/Syntax.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"
#include "AssertingEnumerator.h"
#include "YAC/CodeAnalysis/Syntax/Expression.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxFacts.h"

struct BinaryOperatorPair {
    SyntaxKind op1;
    SyntaxKind op2;

    // Define a proper output operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const BinaryOperatorPair& pair) {
        os << "BinaryOperatorPair{op1=" << syntaxKindToString(pair.op1)
           << ", op2=" << syntaxKindToString(pair.op2) << "}";
        return os;
    }
};

// Test fixture for binary operator precedence tests
struct ParserBinaryExpressionTests : public ::testing::TestWithParam<BinaryOperatorPair> {
};

// Generate all pairs of binary operators
std::vector<BinaryOperatorPair> GetBinaryOperatorPairsData() {
    std::vector<BinaryOperatorPair> pairs;

    auto op1Kinds = SyntaxFacts::GetBinaryOperatorKinds();
    auto op2Kinds = SyntaxFacts::GetBinaryOperatorKinds();

    for (const auto op1 : op1Kinds) {
        for (const auto op2 : op2Kinds) {
            pairs.push_back({op1, op2});
        }
    }

    return pairs;
}

// Parameterized test for binary expression precedence
TEST_P(ParserBinaryExpressionTests, BinaryExpression_HonorsPrecedences) {
    const auto& params = GetParam();
    const SyntaxKind op1 = params.op1;
    const SyntaxKind op2 = params.op2;

    const int op1Precedence = SyntaxFacts::getPrecedence(op1);
    const int op2Precedence = SyntaxFacts::getPrecedence(op2);
    const std::string op1Text = getKindText(op1);
    const std::string op2Text = getKindText(op2);

    const std::string text = "a " + op1Text + " b " + op2Text + " c";
    const auto parser = new Parser(text);
    const auto tree = parser->parse();
    ExpressionSyntax& expression = tree->root();

    if (op1Precedence >= op2Precedence) {
        // Higher precedence tree:
        //     op2
        //    /   \
        //   op1   c
        //  /   \
        // a     b

        AssertingEnumerator e(&expression);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "a");
        e.AssertToken(op1, op1Text);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "b");
        e.AssertToken(op2, op2Text);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "c");
    } else {
        // Lower precedence tree:
        //   op1
        //  /   \
        // a    op2
        //     /   \
        //    b     c

        AssertingEnumerator e(&expression);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "a");
        e.AssertToken(op1, op1Text);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "b");
        e.AssertToken(op2, op2Text);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "c");
    }
}

// Instantiate the test cases
INSTANTIATE_TEST_SUITE_P(
    ParserTests,
    ParserBinaryExpressionTests,
    ::testing::ValuesIn(GetBinaryOperatorPairsData()),
    // Optional test name generator function
    [](const testing::TestParamInfo<BinaryOperatorPair>& info) {
        std::string op1 = syntaxKindToString(info.param.op1);
        std::string op2 = syntaxKindToString(info.param.op2);
        return "Op_" + op1 + "_" + op2;
    }
);


// Test fixture for unary operator precedence tests
struct ParserUnaryExpressionTests : public ::testing::TestWithParam<std::pair<SyntaxKind, SyntaxKind>> {
};

// Generate all pairs of unary and binary operators
std::vector<std::pair<SyntaxKind, SyntaxKind>> GetUnaryOperatorPairsData() {
    std::vector<std::pair<SyntaxKind, SyntaxKind>> pairs;

    auto unaryKinds = SyntaxFacts::GetUnaryOperatorKinds();
    auto binaryKinds = SyntaxFacts::GetBinaryOperatorKinds();

    for (const auto& unary : unaryKinds) {
        for (const auto& binary : binaryKinds) {
            pairs.emplace_back(unary, binary);
        }
    }

    return pairs;
}

// Parameterized test for unary expression precedence
TEST_P(ParserUnaryExpressionTests, UnaryExpression_HonorsPrecedences) {
    const auto& [unaryKind, binaryKind] = GetParam();

    const int unaryPrecedence = SyntaxFacts::getUnaryPrecedence(unaryKind);
    const int binaryPrecedence = SyntaxFacts::getPrecedence(binaryKind);
    const std::string unaryText = getKindText(unaryKind);
    const std::string binaryText = getKindText(binaryKind);

    const std::string text = unaryText + " a " + binaryText + " b";
    const auto parser = new Parser(text);
    const auto tree = parser->parse();
    ExpressionSyntax& expression = tree->root();

    if (unaryPrecedence >= binaryPrecedence) {
        // Higher precedence tree:
        //   binary
        //   /    \
        // unary   b
        //   |
        //   a

        AssertingEnumerator e(&expression);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::UnaryExpression);
        e.AssertToken(unaryKind, unaryText);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "a");
        e.AssertToken(binaryKind, binaryText);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "b");
    } else {
        // Lower precedence tree:
        //  unary
        //    |
        //  binary
        //  /   \
        // a     b

        AssertingEnumerator e(&expression);
        e.AssertNode(SyntaxKind::UnaryExpression);
        e.AssertToken(unaryKind, unaryText);
        e.AssertNode(SyntaxKind::BinaryExpression);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "a");
        e.AssertToken(binaryKind, binaryText);
        e.AssertNode(SyntaxKind::NameExpression);
        e.AssertToken(SyntaxKind::IdentifierToken, "b");
    }
}

// Instantiate the test cases
INSTANTIATE_TEST_SUITE_P(
    ParserTests,
    ParserUnaryExpressionTests,
    ::testing::ValuesIn(GetUnaryOperatorPairsData()),
    [](const testing::TestParamInfo<std::pair<SyntaxKind, SyntaxKind>>& info) {
        std::string unary = syntaxKindToString(info.param.first);
        std::string binary = syntaxKindToString(info.param.second);
        return "Unary_" + unary + "_Binary_" + binary;
    }
);

