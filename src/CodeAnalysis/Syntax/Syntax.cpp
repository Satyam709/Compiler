#include "CodeAnalysis/Syntax/Syntax.h"
#include <iostream>

SyntaxToken::SyntaxToken()
    : position(0), kind(SyntaxKind::BadToken), text(""), val(nullptr) {
}

SyntaxToken::SyntaxToken(const int position, const SyntaxKind kind, const std::string &text, std::any val)
    : position(position), kind(kind), text(text), val(std::move(val)) {
}

SyntaxToken::~SyntaxToken() = default;

// Function to convert SyntaxKind to string
std::string syntaxKindToString(const SyntaxKind kind) {
    switch (kind) {
        case SyntaxKind::NumberToken: return "NumberToken";
        case SyntaxKind::WhitespaceToken: return "WhitespaceToken";
        case SyntaxKind::PlusToken: return "PlusToken";
        case SyntaxKind::MinusToken: return "MinusToken";
        case SyntaxKind::StarToken: return "StarToken";
        case SyntaxKind::SlashToken: return "SlashToken";
        case SyntaxKind::OpenParenthesisToken: return "OpenParenthesisToken";
        case SyntaxKind::CloseParenthesisToken: return "CloseParenthesisToken";
        case SyntaxKind::BadToken: return "BadToken";
        case SyntaxKind::EndOfFileToken: return "EndOfFileToken";
        case SyntaxKind::LiteralExpression: return "LiteralExpression";
        case SyntaxKind::BinaryExpression: return "BinaryExpression";
        case SyntaxKind::ParenthesizedExpression: return "ParenthesizedExpression";
        case SyntaxKind::UnaryExpression: return "UnaryExpression";
        case SyntaxKind::FalseKeyword: return "FalseKeyword";
        case SyntaxKind::TrueKeyword: return "TrueKeyword";
        case SyntaxKind::IdentifierToken: return "IdentifierToken";
        case SyntaxKind::BangToken: return "BangToken";
        case SyntaxKind::PipePipeToken: return "PipePipeToken";
        case SyntaxKind::AmpersandAmpersandToken: return "AmpersandAmpersandToken";
        default: return "UnknownToken";
    }
}




std::ostream &operator<<(std::ostream &out, const SyntaxToken &token) {
    out << "text: " << token.text
            << " position: " << token.position
            << " kind: " << syntaxKindToString(token.kind);
    return out;
}
