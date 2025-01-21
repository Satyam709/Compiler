#ifndef SYNTAX_H
#define SYNTAX_H
#include <any>
#include <string>
#include <ostream>

enum SyntaxKind {

    NumberToken,
    WhitespaceToken,
    PlusToken,
    MinusToken,
    StarToken,
    SlashToken,
    OpenParenthesisToken,
    CloseParenthesisToken,

    BadToken,
    EndOfFileToken,

    // expressions
    LiteralExpression,
    BinaryExpression,
    ParenthesizedExpression,
    UnaryExpression
};

// just a helper method to convert enum kind from int to respective value
std::string syntaxKindToString(SyntaxKind kind);

class SyntaxToken {
public:
    SyntaxToken();
    SyntaxToken(int position, SyntaxKind kind, std::string text, std::any val);
    ~SyntaxToken();
    int position;
    SyntaxKind kind;
    std::string text{};
    std::any val;
};


// Overload operator<< for SyntaxToken
std::ostream &operator<<(std::ostream &out, const SyntaxToken &token);

#endif //SYNTAX_H
