#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <vector>
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const std::string& input);
    SyntaxTree *parse();

    static std::vector<SyntaxToken> getTokens(const std::string &input);

private:
    std::vector<SyntaxToken> _tokens;
    DiagnosticBag *_diagnostics;
    int _position;

    SyntaxToken peek(int offset);
    SyntaxToken current();
    SyntaxToken nextToken();
    SyntaxToken match(SyntaxKind kind);

    ExpressionSyntax *parseExpression();
    ExpressionSyntax *parseAssignmentExpression();
    ExpressionSyntax *parseBinaryExpression(int parentPrecedence = 0);
    ExpressionSyntax *parsePrimaryExpression();
};
#endif //PARSER_H
