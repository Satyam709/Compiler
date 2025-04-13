#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <vector>
#include <YAC/CodeAnalysis/Text/SourceText.h>

#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const SourceText& input);
    SyntaxTree *parse() const;

    static std::vector<SyntaxToken> getTokens(const SourceText &input);

private:
    std::vector<SyntaxToken> _tokens;
    DiagnosticBag *_diagnostics;
    int _position;
    SourceText _text;

    SyntaxToken peek(int offset) const;
    SyntaxToken current() const;
    SyntaxToken nextToken();
    SyntaxToken match(SyntaxKind kind);

    ExpressionSyntax *parseExpression() const;
    ExpressionSyntax *parseAssignmentExpression() const;
    ExpressionSyntax *parseBinaryExpression(int parentPrecedence = 0) const;
    ExpressionSyntax *parsePrimaryExpression() const;
};
#endif //PARSER_H
