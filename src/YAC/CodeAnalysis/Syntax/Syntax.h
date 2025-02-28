#ifndef SYNTAX_H
#define SYNTAX_H
#include <any>
#include <string>
#include <ostream>

#include "YAC/CodeAnalysis/Text/TextSpan.h"

enum class SyntaxKind {

    NumberToken,
    WhitespaceToken,
    PlusToken,
    MinusToken,
    StarToken,
    SlashToken,
    EqualsToken,
    OpenParenthesisToken,
    CloseParenthesisToken,

    // logical operators
    BangToken,
    AmpersandAmpersandToken,
    PipePipeToken,

    BadToken,
    EndOfFileToken,

    // expressions
    LiteralExpression,
    BinaryExpression,
    ParenthesizedExpression,
    UnaryExpression,
    NameExpression,
    AssignmentExpression,

    // keywords

    // bools
    TrueKeyword,
    FalseKeyword,

    // identifier

    IdentifierToken,
    EqualEqualToken,
    NotEqualToken,



    // auxiliary kind to mark enum end
    ENDS
};

// just a helper method to convert enum class kind from int to respective value
std::string syntaxKindToString(SyntaxKind kind);

// converts a kind to its actual representation  e.g. PlusToken -> "+"
std::string getKindText(const SyntaxKind& kind);

class SyntaxToken {
public:
    SyntaxToken();
    SyntaxToken(int position, SyntaxKind kind, const std::string &text, std::any val);
    ~SyntaxToken();
    int position;
    SyntaxKind kind;
    std::string text{};
    std::any val;
    TextSpan getSpan() const ;

};


// Overload operator<< for SyntaxToken
std::ostream &operator<<(std::ostream &out, const SyntaxToken &token);

// Overload operator<< for SyntaxKind
inline std::ostream& operator<<(std::ostream &out, const SyntaxKind kind) {
    out << syntaxKindToString(kind);
    return out;
}

#endif //SYNTAX_H
