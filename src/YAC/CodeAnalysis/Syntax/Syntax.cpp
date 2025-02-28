#include "Syntax.h"
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
        case SyntaxKind::EqualEqualToken: return "EqualEqualToken";
        case SyntaxKind::NotEqualToken: return "NotEqualToken";
        case SyntaxKind::EqualsToken: return "EqualsToken";                // Added
        case SyntaxKind::NameExpression: return "NameExpression";         // Added
        case SyntaxKind::AssignmentExpression: return "AssignmentExpression"; // Added
        default: return "UnknownToken";
    }
}

std::string getKindText(const SyntaxKind& kind) {
    switch (kind) {
        case SyntaxKind::PlusToken:
            return "+";
        case SyntaxKind::MinusToken:
            return "-";
        case SyntaxKind::StarToken:
            return "*";
        case SyntaxKind::SlashToken:
            return "/";
        case SyntaxKind::BangToken:
            return "!";
        case SyntaxKind::EqualsToken:
            return "=";
        case SyntaxKind::AmpersandAmpersandToken:
            return "&&";
        case SyntaxKind::PipePipeToken:
            return "||";
        case SyntaxKind::EqualEqualToken:
            return "==";
        case SyntaxKind::NotEqualToken:
            return "!=";
        case SyntaxKind::OpenParenthesisToken:
            return "(";
        case SyntaxKind::CloseParenthesisToken:
            return ")";
        case SyntaxKind::FalseKeyword:
            return "false";
        case SyntaxKind::TrueKeyword:
            return "true";
        default:
            return "";
    }
}

TextSpan SyntaxToken::getSpan() const {
    int len = text.length();
    return {position, len};
}

std::ostream &operator<<(std::ostream &out, const SyntaxToken &token) {
    out << "text: " << token.text
            << " position: " << token.position
            << " kind: " << syntaxKindToString(token.kind);
    return out;
}