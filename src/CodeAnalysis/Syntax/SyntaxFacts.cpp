//
// Created by satya on 01-01-2025.
//

#include "CodeAnalysis/Syntax/SyntaxFacts.h"

#include <unordered_map>

int SyntaxFacts::getPrecedence(const SyntaxKind &kind) {
    switch (kind) {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 1;
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 2;
        default: return 0;
    }
}

int SyntaxFacts::getUnaryPrecedence(const SyntaxKind &kind) {
    switch (kind) {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 3;
        default: return 0;
    }
}

// SyntaxKind SyntaxFacts::getKeywordKind(const std::string& text) {
//     switch (text) {
//         case "true":
//             return SyntaxKind::TrueKeyword;
//         case "false":
//             return SyntaxKind::FalseKeyword;
//         default:
//             return SyntaxKind::IdentifierToken;
//     }
// }

SyntaxKind SyntaxFacts::getKeywordKind(const std::string &text) {
    static const std::unordered_map<std::string, SyntaxKind> keywords = {
        {"true", SyntaxKind::TrueKeyword},
        {"false", SyntaxKind::FalseKeyword}
    };

    const auto it = keywords.find(text);
    return it != keywords.end() ? it->second : SyntaxKind::IdentifierToken;
}
