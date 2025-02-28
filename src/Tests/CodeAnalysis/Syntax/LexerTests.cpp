#include <gtest/gtest.h>
#include <utility>
#include "YAC/CodeAnalysis/Syntax/Parser.h"
#include "YAC/CodeAnalysis/Syntax/Syntax.h"

struct TokenInfo {
    std::string text;
    SyntaxKind kind;

    TokenInfo(const SyntaxKind kind, std::string text) : text(std::move(text)), kind(kind) {}

    // Define a proper friend function for ostream operator<<
    friend std::ostream &operator<<(std::ostream &o, const TokenInfo &token) {
        const auto tokenText = syntaxKindToString(token.kind);
        o << "TokenInfo { kind: " << tokenText << ", text: \"" << token.text << "\" }";
        return o;
    }
};

// Add a Google Test PrintTo function to display TokenInfo properly
void PrintTo(const TokenInfo &token, std::ostream *os) {
    *os << token; // Use the operator<< we just defined
}

static std::vector<TokenInfo> GetTokens() {
    return
    {
        {SyntaxKind::PlusToken, "+"},
        {SyntaxKind::StarToken, "*"},
        {SyntaxKind::MinusToken, "-"},
        {SyntaxKind::SlashToken, "/"},
        {SyntaxKind::BangToken, "!"},
        {SyntaxKind::EqualsToken, "+"},
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

// Parameterized fixture
struct LexerTests : testing::TestWithParam<TokenInfo> { };

TEST_P(LexerTests, LexerLexesTokens) {
    const auto text = GetParam().text;
    const auto kind = GetParam().kind;

    const auto tokens = Parser::getTokens(text);
    ASSERT_FALSE(tokens.empty()) << "Lexer returned no tokens for input: " << text;

    const auto &lexed = tokens.front(); // Get the top element as it contains 'EOF' token too
    ASSERT_EQ(tokens.size(), 2) << "Expected {token, EOF} but got " << tokens.size() << " tokens";

    EXPECT_EQ(lexed.text, text) << "Mismatch in lexed text for input: " << text;
    EXPECT_EQ(lexed.kind, kind) << "Mismatch in token kind for input: " << text;
}

INSTANTIATE_TEST_SUITE_P(
    LexerLexesTokens, // Test suite name
    LexerTests, // Test fixture
    ::testing::ValuesIn(GetTokens()) // Pass each token as a test case
);

