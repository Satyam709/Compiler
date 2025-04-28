#include <unordered_set>
#include <gtest/gtest.h>
#include <utility>
#include <vector>

#include "YAC/CodeAnalysis/Syntax/Parser.h"
#include "YAC/CodeAnalysis/Syntax/Syntax.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxFacts.h"

struct TokenInfo {
    std::string text;
    SyntaxKind kind;

    TokenInfo(const SyntaxKind kind, std::string text) : text(std::move(text)), kind(kind) {
    }

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

static std::vector<std::tuple<SyntaxKind, std::string>> GetTokens() {
    // Get fixed tokens
    std::vector<std::tuple<SyntaxKind, std::string>> fixedTokens;

    // Manually list all SyntaxKind values or use a similar approach if possible
    std::vector<SyntaxKind> kinds = {
        SyntaxKind::PlusToken, SyntaxKind::StarToken, SyntaxKind::MinusToken,
        SyntaxKind::SlashToken, SyntaxKind::BangToken, SyntaxKind::EqualsToken,
        SyntaxKind::AmpersandAmpersandToken, SyntaxKind::PipePipeToken,
        SyntaxKind::EqualEqualToken, SyntaxKind::NotEqualToken,
        SyntaxKind::OpenParenthesisToken, SyntaxKind::CloseParenthesisToken,
        SyntaxKind::FalseKeyword, SyntaxKind::TrueKeyword,
        // Add other SyntaxKind values as needed
    };

    for (const auto& kind : kinds) {
        std::string text = getKindText(kind);
        if (!text.empty()) {
            fixedTokens.emplace_back(kind, text);
        }
    }

    // Get dynamic tokens
    std::vector<std::tuple<SyntaxKind, std::string>> dynamicTokens = {
        {SyntaxKind::NumberToken, "1"},
        {SyntaxKind::NumberToken, "123"},
        {SyntaxKind::IdentifierToken, "a"},
        {SyntaxKind::IdentifierToken, "abc"},
    };

    // Combine fixed and dynamic tokens
    fixedTokens.insert(fixedTokens.end(), dynamicTokens.begin(), dynamicTokens.end());
    return fixedTokens;
}

static bool isKeyword(const SyntaxKind kind) {
    const auto txt = syntaxKindToString(kind);
    if (txt.ends_with("Keyword"))return true; // currently only true/false keyword
    return false;
}

static bool RequiresSeparator(const SyntaxKind t1Kind, const SyntaxKind t2Kind) {
    const bool t1IsKeyword = isKeyword(t1Kind);
    const bool t2IsKeyword = isKeyword(t2Kind);

    if (t1Kind == SyntaxKind::IdentifierToken && t2Kind == SyntaxKind::IdentifierToken)
        return true;

    if (t1IsKeyword && t2IsKeyword)
        return true;

    if (t1IsKeyword && t2Kind == SyntaxKind::IdentifierToken)
        return true;

    if (t1Kind == SyntaxKind::IdentifierToken && t2IsKeyword)
        return true;

    if (t1Kind == SyntaxKind::NumberToken && t2Kind == SyntaxKind::NumberToken)
        return true;

    if (t1Kind == SyntaxKind::BangToken && t2Kind == SyntaxKind::EqualsToken)
        return true;

    if (t1Kind == SyntaxKind::BangToken && t2Kind == SyntaxKind::EqualEqualToken)
        return true;

    if (t1Kind == SyntaxKind::EqualsToken && t2Kind == SyntaxKind::EqualsToken)
        return true;

    if (t1Kind == SyntaxKind::EqualsToken && t2Kind == SyntaxKind::EqualEqualToken)
        return true;

    if (t1Kind == SyntaxKind::LessToken && t2Kind == SyntaxKind::EqualsToken)
        return true;

    if (t1Kind == SyntaxKind::LessToken && t2Kind == SyntaxKind::EqualEqualToken)
        return true;

    if (t1Kind == SyntaxKind::GreaterToken && t2Kind == SyntaxKind::EqualsToken)
        return true;

    if (t1Kind == SyntaxKind::GreaterToken && t2Kind == SyntaxKind::EqualEqualToken)
        return true;

    if (t1Kind == SyntaxKind::AmpersandToken && t2Kind == SyntaxKind::AmpersandToken)
        return true;

    if (t1Kind == SyntaxKind::AmpersandToken && t2Kind == SyntaxKind::AmpersandAmpersandToken)
        return true;

    if (t1Kind == SyntaxKind::PipeToken && t2Kind == SyntaxKind::PipeToken)
        return true;

    if (t1Kind == SyntaxKind::PipeToken && t2Kind == SyntaxKind::PipePipeToken)
        return true;

    return false;
}


static auto GetTokenPairs() {
    std::vector<std::vector<TokenInfo>> pairedTokens = {};

    for (const auto &t1: GetTokens()) {
        for (const auto &t2: GetTokens()) {
            if (!RequiresSeparator(std::get<0>(t1), std::get<0>(t2))) {
                pairedTokens.emplace_back(std::vector<TokenInfo>{
                    TokenInfo(std::get<0>(t1), std::get<1>(t1)),
                    TokenInfo(std::get<0>(t2), std::get<1>(t2))
                });
            }
        }
    }
    return pairedTokens;
}


// Parameterized fixture
struct LexerTests : testing::TestWithParam<TokenInfo> {
};

TEST(LexerTests, LexerTestsAllTokens) {
    // Get all token kinds that end with "Keyword" or "Token"
    std::vector<SyntaxKind> tokenKinds;
    for (int i = static_cast<int>(SyntaxKind::BadToken); i <= static_cast<int>(SyntaxKind::EndOfFileToken); ++i) {
        SyntaxKind kind = static_cast<SyntaxKind>(i);
        std::string kindStr = syntaxKindToString(kind);
        if (kindStr.ends_with("Keyword") || kindStr.ends_with("Token")) {
            tokenKinds.push_back(kind);
        }
    }

    // Collect tested token kinds
    std::vector<SyntaxKind> testedTokenKinds;
    auto tokens = GetTokens();
    for (const auto& token : tokens) {
        // Here we need to extract the kind from the tuple
        testedTokenKinds.push_back(std::get<0>(token));
    }

    // Create a set of untested token kinds
    std::set<SyntaxKind> untestedTokenKinds(tokenKinds.begin(), tokenKinds.end());
    untestedTokenKinds.erase(SyntaxKind::BadToken);
    untestedTokenKinds.erase(SyntaxKind::EndOfFileToken);

    // Remove tested kinds from untested
    for (const auto& kind : testedTokenKinds) {
        untestedTokenKinds.erase(kind);
    }

    // Assert that the set of untested kinds is empty
    EXPECT_TRUE(untestedTokenKinds.empty()) << "Untested token kinds: ";
    for (const auto& kind : untestedTokenKinds) {
        std::cout << syntaxKindToString(kind) << ", ";
    }
    std::cout << std::endl;
}


// single token test
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

// paired tokens tests

struct LexerPairedTests : testing::TestWithParam<std::vector<TokenInfo>>{
};

// Test Token Pairs
TEST_P(LexerPairedTests, LexerLexesPairedTokens) {
    const auto params = GetParam();
    const auto& t1 = params[0];
    const auto& t2 = params[1];
    const std::string text = t1.text + t2.text; // Concatenate tokens

    const auto tokens = Parser::getTokens(text);

    ASSERT_EQ(tokens.size(), 3) << "Expected {token1, token2, EOF} but got " << tokens.size() << " tokens";

    EXPECT_EQ(tokens[0].text, t1.text) << "First token mismatch for input: " << text;
    EXPECT_EQ(tokens[0].kind, t1.kind) << "First token kind mismatch for input: " << text;

    EXPECT_EQ(tokens[1].text, t2.text) << "Second token mismatch for input: " << text;
    EXPECT_EQ(tokens[1].kind, t2.kind) << "Second token kind mismatch for input: " << text;
}

INSTANTIATE_TEST_SUITE_P(
    LexerLexesTokens, // Test suite name
    LexerTests, // Test fixture
    ::testing::ValuesIn(
        // Convert each tuple to TokenInfo
        [&]() {
            std::vector<TokenInfo> tokens;
            for (const auto& token : GetTokens()) {
                tokens.emplace_back(std::get<0>(token), std::get<1>(token));
            }
            return tokens;
        }()
    ) // Pass each token as a test case
);

INSTANTIATE_TEST_SUITE_P(
    LexerLexesPairedTokens,
    LexerPairedTests,
    testing::ValuesIn(GetTokenPairs()) // Paired token test cases
);
