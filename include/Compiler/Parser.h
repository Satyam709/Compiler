#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "Compiler/Syntax.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const std::string_view input);
    SyntaxTree *parse();
private:
    std::vector<SyntaxToken> _tokens;
    std::vector<std::string_view> _diagnostics;
    int _position;

    SyntaxToken peek(const int offset);

    SyntaxToken current();

    SyntaxToken nextToken();

    SyntaxToken match(SyntaxKind kind);

    ExpressionSyntax *parseExpression();

    ExpressionSyntax *parsePrimaryExpression();

    ExpressionSyntax *parseFactor();

    ExpressionSyntax *parseTerm();


};
#endif //PARSER_H
