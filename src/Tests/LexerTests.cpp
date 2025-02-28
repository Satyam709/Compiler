#include <gtest/gtest.h>
#include <utility>
#include "YAC/CodeAnalysis/Syntax/Parser.h"
#include "YAC/CodeAnalysis/Syntax/Syntax.h"

struct TokenInfo {
    std::string text;
    SyntaxKind kind;

    TokenInfo(const SyntaxKind kind, std::string text) : text(std::move(text)), kind(kind) {
    }
};


static std::vector<TokenInfo> GetTokens() {
    return
    {
        {SyntaxKind::PlusToken, "+"},
        {SyntaxKind::StarToken, "*"},
        {SyntaxKind::MinusToken, "-"},
        {SyntaxKind::SlashToken, "/"},
        {SyntaxKind::BangToken, "!"},
        {SyntaxKind::EqualsToken, "="},
        {SyntaxKind::AmpersandAmpersandToken, "&&"},
        {SyntaxKind::PipePipeToken, "||"},
        {SyntaxKind::EqualEqualToken, "=="},
        {SyntaxKind::NotEqualToken, "!="},
        {SyntaxKind::OpenParenthesisToken, "("},
        {SyntaxKind::CloseParenthesisToken, ")"},
        {SyntaxKind::FalseKeyword, "false"},
        {SyntaxKind::TrueKeyword, "true"},
        {SyntaxKind::NumberToken, "1"},
        {SyntaxKind::NumberToken, "123"},
        {SyntaxKind::IdentifierToken, "a"},
        {SyntaxKind::IdentifierToken, "abc"},
    };
}


// parameterized fixture
struct LexerTests : testing::TestWithParam<TokenInfo> {};


TEST_P(LexerTests, LexerLexesTokens) {
    const auto text = GetParam().text;
    const auto kind = GetParam().kind;

    const auto tokens = Parser::getTokens(text);
    const auto& lexed = tokens.front(); //get the top element as it contains 'EOF' token too
    EXPECT_TRUE(tokens.size()==2); // {token,eof}
    EXPECT_EQ(lexed.text, text);
    EXPECT_EQ(lexed.kind, kind);
}

INSTANTIATE_TEST_SUITE_P(
    LexerLexesTokens, // Test suite name
    LexerTests, // Test fixture
    ::testing::ValuesIn(GetTokens()) // Pass each token as a test case
);
