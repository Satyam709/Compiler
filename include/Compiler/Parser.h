#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "Compiler/Syntax.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const std::string input);
    SyntaxTree *parse();
private:
    std::vector<SyntaxToken> _tokens;
    std::vector<std::string> _diagnostics;
    int _position;

    SyntaxToken peek(const int offset);

    SyntaxToken current();

    SyntaxToken nextToken();

    SyntaxToken match(SyntaxKind kind);

    ExpressionSyntax *parseExpression(int parentPrecedence = 0);

    ExpressionSyntax *parsePrimaryExpression();


};
#endif //PARSER_H
