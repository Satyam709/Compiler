//
// Created by satya on 28-02-2025.
//

#include <gtest/gtest.h>

#include "YAC/CodeAnalysis/Syntax/Parser.h"
#include "YAC/CodeAnalysis/Syntax/Syntax.h"

static auto GetSyntaxKindData() {
    std::vector<SyntaxKind> data ={};

    for (auto i = 0; i < static_cast<int>(SyntaxKind::ENDS); i++) {
        data.emplace_back(static_cast<SyntaxKind>(i)); // cast back
    }
    return data;
}


// param fixture for kind to text test
struct SyntaxFactsKindTextTest : testing::TestWithParam<SyntaxKind> {
};

TEST_P(SyntaxFactsKindTextTest, SyntaxKindToTextTest) {
    const auto kind = GetParam();

    const  auto text = getKindText(kind);

    if (text.empty()) {
        return;
    }
    const auto tokens = Parser::getTokens(text);

    ASSERT_FALSE(tokens.empty()) << "Lexer returned no tokens for input: " << text;
    const auto &lexed = tokens.front(); // Get the top element as it contains 'EOF' token too
    ASSERT_EQ(tokens.size(), 2) << "Expected {token, EOF} but got " << tokens.size() << " tokens";
    EXPECT_EQ(lexed.text, text) << "Mismatch in lexed text for input: " << text;
    EXPECT_EQ(lexed.kind, kind) << "Mismatch in token kind for input: " << text;
}


INSTANTIATE_TEST_SUITE_P(
SyntaxKindToTextTest,
    SyntaxFactsKindTextTest,
    testing::ValuesIn(GetSyntaxKindData())
);