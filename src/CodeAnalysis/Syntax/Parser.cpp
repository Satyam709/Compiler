#include "CodeAnalysis/Syntax/Parser.h"
#include <format>
#include <iostream>
#include <sstream>
#include <vector>

#include "CodeAnalysis/Syntax/Expression.h"
#include "CodeAnalysis/Syntax/Lexer.h"
#include "CodeAnalysis/Syntax/Syntax.h"
#include "CodeAnalysis/Syntax/SyntaxFacts.h"


Parser::Parser(const std::string input) {
    Lexer lexer(input);
    auto tokenList = lexer.tokenize();
    _tokens = std::vector(tokenList.begin(), tokenList.end()); // Convert list to vector

    // std::cout << "tokenized string !"<<std::endl;
    // for (auto token : _tokens) {
    //     std::cout << token<<std::endl;
    // }

    _position = 0;
}

SyntaxToken Parser::peek(const int offset) {
    if (_position + offset < _tokens.size())
        return _tokens[_position + offset];
    return _tokens[_tokens.size() - 1];
}

SyntaxToken Parser::current() { return peek(0); };

SyntaxToken Parser::nextToken() {
    auto token = current();
    _position++;
    return token;
}

SyntaxToken Parser::match(SyntaxKind kind) {
    const auto token = current();
    if (token.kind == kind)return nextToken();
    std::ostringstream os;
    os << "ERROR: Unexpected token <" << token
            << ">, expected <" << syntaxKindToString(kind) << ">";
    _diagnostics.push_back(os.str());
    return {_position, kind, "", nullptr};
}

ExpressionSyntax *Parser::parseExpression(const int parentPrecedence) {
    ExpressionSyntax *left;

    if (const int unaryOperatorPrecedence = SyntaxFacts::getUnaryPrecedence(current().kind);
        unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPrecedence) {
        const SyntaxToken operatorToken = nextToken();
        auto* operand = parseExpression(unaryOperatorPrecedence);
        left = new UnaryExpressionSyntax(operatorToken, *operand);
    } else {
        left = parsePrimaryExpression();
    }

    while (true) {
        const int currentPrecedence = SyntaxFacts::getPrecedence(current().kind);
        if (currentPrecedence == 0 || currentPrecedence <= parentPrecedence)
            break;

        auto operatorToken = nextToken();
        auto *right = parseExpression(currentPrecedence);
        left = new BinaryExpressionSyntax(*left, operatorToken, *right);
    }
    return left;
}

ExpressionSyntax *Parser::parsePrimaryExpression() {
    auto crnt = current();
    if (crnt.kind == OpenParenthesisToken) {
        auto left = nextToken();
        auto expression = parseExpression();
        auto right = match(CloseParenthesisToken);
        return new ParenthesizedExpressionSyntax(left, *expression, right);
    }

    auto numberToken = match(NumberToken);
    return new LiteralExpressionSyntax(numberToken);
}

SyntaxTree *Parser::parse() {
    auto expression = parseExpression(0);
    auto endOfFileToken = match(EndOfFileToken);
    return new SyntaxTree(_diagnostics, *expression, endOfFileToken);
}
