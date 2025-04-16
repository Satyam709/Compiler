#include "SyntaxFacts.h"
#include <unordered_map>
#include <vector>

int SyntaxFacts::getUnaryPrecedence(const SyntaxKind &kind) {
    switch (kind) {
        case SyntaxKind::BangToken:
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 6;

        default:
            return 0;
    }
}

int SyntaxFacts::getPrecedence(const SyntaxKind &kind) {
    switch (kind) {
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 5;

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 4;

        case SyntaxKind::EqualEqualToken:
        case SyntaxKind::NotEqualToken:
            return 3;

        case SyntaxKind::AmpersandAmpersandToken:
            return 2;

        case SyntaxKind::PipePipeToken:
            return 1;

        case SyntaxKind::EqualsToken:
            return 0;

        default:
            return 0;
    }
}

SyntaxKind SyntaxFacts::getKeywordKind(const std::string &text) {
    static const std::unordered_map<std::string, SyntaxKind> keywords = {
        {"true", SyntaxKind::TrueKeyword},
        {"false", SyntaxKind::FalseKeyword},
        {"let",SyntaxKind::LetKeyword},
        {"var",SyntaxKind::VarKeyword}
    };

    const auto it = keywords.find(text);
    return it != keywords.end() ? it->second : SyntaxKind::IdentifierToken;
}

std::vector<SyntaxKind> SyntaxFacts::GetUnaryOperatorKinds() {
    std::vector<SyntaxKind> result;
    for (int i = 0; i < static_cast<int>(SyntaxKind::ENDS); i++) {
        const auto kind = static_cast<SyntaxKind>(i);
        if (getUnaryPrecedence(kind) > 0) {
            result.push_back(kind);
        }
    }
    return result;
}

std::vector<SyntaxKind> SyntaxFacts::GetBinaryOperatorKinds() {
    std::vector<SyntaxKind> result;
    for (int i = 0; i < static_cast<int>(SyntaxKind::ENDS); i++) {
        const auto kind = static_cast<SyntaxKind>(i);
        if (getPrecedence(kind) > 0) {
            result.push_back(kind);
        }
    }
    return result;
}

