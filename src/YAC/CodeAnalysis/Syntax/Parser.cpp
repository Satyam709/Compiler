#include "Parser.h"
#include <format>
#include <vector>
#include "Expression.h"
#include "Lexer.h"
#include "Syntax.h"
#include "SyntaxFacts.h"

Parser::Parser(const SourceText& input) : _text(input) {
    Lexer lexer(input);
    auto tokenList = lexer.tokenize();
    _tokens = std::vector(tokenList.begin(), tokenList.end());
    const auto lex_diag = lexer.diagnostic_bag();
    _diagnostics = lex_diag;
    _position = 0;
}

SyntaxToken Parser::peek(const int offset) const {
    if (_position + offset < _tokens.size())
        return _tokens[_position + offset];
    return _tokens[_tokens.size() - 1];
}

SyntaxToken Parser::current() const {
    return peek(0);
}

SyntaxToken Parser::nextToken() {
    auto token = current();
    _position++;
    return token;
}

SyntaxToken Parser::match(SyntaxKind kind) {
    const auto token = current();
    if (token.kind == kind)
        return nextToken();
    _diagnostics->reportUnexpectedToken(token.getSpan(), token.kind, kind);
    return {_position, kind, "", nullptr};
}

ExpressionSyntax* Parser::parseExpression() const {
    return parseAssignmentExpression();
}

ExpressionSyntax* Parser::parseAssignmentExpression() const {
    if (peek(0).kind == SyntaxKind::IdentifierToken &&
        peek(1).kind == SyntaxKind::EqualsToken) {
        auto* nonConstThis = const_cast<Parser*>(this);
        const auto identifierToken = nonConstThis->nextToken();
        const auto operatorToken = nonConstThis->nextToken();
        const auto right = parseAssignmentExpression();
        return new AssignmentExpressionSyntax(identifierToken, operatorToken, *right);
    }

    return parseBinaryExpression();
}

ExpressionSyntax* Parser::parseBinaryExpression(const int parentPrecedence) const {
    ExpressionSyntax* left;
    auto* nonConstThis = const_cast<Parser*>(this);

    if (const int unaryOperatorPrecedence = SyntaxFacts::getUnaryPrecedence(current().kind);
        unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPrecedence) {
        const SyntaxToken operatorToken = nonConstThis->nextToken();
        auto* operand = parseBinaryExpression(unaryOperatorPrecedence);
        left = new UnaryExpressionSyntax(operatorToken, *operand);
    } else {
        left = parsePrimaryExpression();
    }

    while (true) {
        const int currentPrecedence = SyntaxFacts::getPrecedence(current().kind);
        if (currentPrecedence == 0 || currentPrecedence <= parentPrecedence)
            break;
        const auto operatorToken = nonConstThis->nextToken();
        auto* right = parseBinaryExpression(currentPrecedence);
        left = new BinaryExpressionSyntax(*left, operatorToken, *right);
    }
    return left;
}

ExpressionSyntax* Parser::parsePrimaryExpression() const {
    auto* nonConstThis = const_cast<Parser*>(this);
    switch (current().kind) {
        case SyntaxKind::OpenParenthesisToken: {
            const auto left = nonConstThis->nextToken();
            const auto expression = parseExpression();
            const auto right = nonConstThis->match(SyntaxKind::CloseParenthesisToken);
            return new ParenthesizedExpressionSyntax(left, *expression, right);
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword: {
            const auto keywordToken = nonConstThis->nextToken();
            bool val = keywordToken.kind == SyntaxKind::TrueKeyword;
            return new LiteralExpressionSyntax(keywordToken, val);
        }
        case SyntaxKind::IdentifierToken: {
            const auto identifierToken = nonConstThis->nextToken();
            return new NameExpressionSyntax(identifierToken);
        }
        default: {
            const auto numberToken = nonConstThis->match(SyntaxKind::NumberToken);
            return new LiteralExpressionSyntax(numberToken);
        }
    }
}

SyntaxTree* Parser::parse() const {
    auto* nonConstThis = const_cast<Parser*>(this);
    const auto expression = parseExpression();
    const auto endOfFileToken = nonConstThis->match(SyntaxKind::EndOfFileToken);
    return new SyntaxTree(_text, _diagnostics, *expression, endOfFileToken);
}

std::vector<SyntaxToken> Parser::getTokens(const SourceText& input) {
    Lexer lexer(input);
    auto tokenList = lexer.tokenize();
    return std::vector(tokenList.begin(), tokenList.end());
}