#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "Compiler/Syntax.h"
#include "Compiler/Expression.h"
#include "SyntaxFacts.h"
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

    // ExpressionSyntax *parseExpression();

    ExpressionSyntax *parsePrimaryExpression();

    // ExpressionSyntax *parseFactor();

    // ExpressionSyntax *parseTerm();

    ExpressionSyntax *parseExpression(int parentPrecedence = 0){
        ExpressionSyntax* left;
        auto unaryOperatorPrecedence = SyntaxFacts::getUnaryOperatorPrecedence(current().kind);
        if(unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPrecedence){
            auto operatorToken = nextToken();
            auto operand = parseExpression(unaryOperatorPrecedence);
            left = new UnaryExpressionSyntax(operatorToken, *operand);
        }
        else {
            left = parsePrimaryExpression();
        }

        while(true){
            auto precedence = SyntaxFacts::getBinaryOperatorPrecedence(current().kind);
            if(precedence == 0 || precedence <= parentPrecedence){
                break;
            }

            auto operatorToken = nextToken();
            auto right = parseExpression(precedence);
            left = new BinaryExpressionSyntax(*left, operatorToken, *right);
        }
        return left;
    }

};
#endif //PARSER_H
