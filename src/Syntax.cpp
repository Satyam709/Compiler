#include "Compiler/Syntax.h"
#include <iostream>

SyntaxToken::SyntaxToken()
    : position(0), kind(SyntaxKind::BadToken), text(""), val(nullptr) {
}

SyntaxToken::SyntaxToken(int position, SyntaxKind kind, std::string_view text, std::any val)
    : position(position), kind(kind), text(text), val(val) {
}

SyntaxToken::~SyntaxToken() = default;

// Function to convert SyntaxKind to string
std::string syntaxKindToString(SyntaxKind kind) {
    switch (kind) {
        case NumberToken: return "NumberToken";
        case WhitespaceToken: return "WhitespaceToken";
        case PlusToken: return "PlusToken";
        case MinusToken: return "MinusToken";
        case StarToken: return "StarToken";
        case SlashToken: return "SlashToken";
        case OpenParenthesisToken: return "OpenParenthesisToken";
        case CloseParenthesisToken: return "CloseParenthesisToken";
        case BadToken: return "BadToken";
        case EndOfFileToken: return "EndOfFileToken";
        case LiteralExpression: return "LiteralExpression";
        case BinaryExpression: return "BinaryExpression";
        case ParenthesizedExpression: return "ParenthesizedExpression";
        case UnaryExpression: return "UnaryExpression";
        default: return "UnknownToken";
    }
}




std::ostream &operator<<(std::ostream &out, const SyntaxToken &token) {
    out << "text: " << token.text
            << " position: " << token.position
            << " kind: " << syntaxKindToString(token.kind);
    return out;
}
