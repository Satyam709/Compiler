#include "Compiler/Parser.h"
#include <format>
#include <iostream>
#include <sstream>
#include <vector>

#include "Compiler/Expression.h"
#include "Compiler/Lexer.h"
#include "Compiler/Syntax.h"

Parser::Parser(const std::string_view input) {
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

ExpressionSyntax* Parser::parseExpression() {
    return parseTerm();
}

ExpressionSyntax* Parser::parsePrimaryExpression() {
    auto crnt = current();
    if (crnt.kind == OpenParenthesisToken) {
        auto left = nextToken();
        auto expression = parseExpression();
        auto right = match(CloseParenthesisToken);
        return new ParenthesizedExpressionSyntax(left, *expression, right);
    }

    auto numberToken = match(NumberToken);
    return new NumberExpressionSyntax(numberToken);
}

ExpressionSyntax* Parser::parseFactor() {
    auto left = parsePrimaryExpression();

    while (current().kind == StarToken || current().kind == SlashToken) {  // Use current() instead of crnt
        auto operatorToken = nextToken();
        auto right = parsePrimaryExpression();
        left = new BinaryExpressionSyntax(*left, operatorToken, *right);
    }
    return left;
}

ExpressionSyntax* Parser::parseTerm() {
    auto left = parseFactor();

    while (current().kind == PlusToken || current().kind == MinusToken) {  // Use current() instead of crnt
        auto operatorToken = nextToken();
        auto right = parseFactor();
        left = new BinaryExpressionSyntax(*left, operatorToken, *right);
    }
    return left;
}

SyntaxTree* Parser::parse() {
    auto expression = parseExpression();
    auto endOfFileToken = match(EndOfFileToken);
    return new SyntaxTree(_diagnostics, *expression, endOfFileToken);
}