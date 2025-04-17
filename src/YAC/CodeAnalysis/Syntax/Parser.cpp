#include "Parser.h"
#include <format>
#include <vector>

#include "BlockStatementSyntax.h"
#include "Expression.h"
#include "Lexer.h"
#include "Syntax.h"
#include "SyntaxFacts.h"
#include "CompilationUnitSyntax.h"
#include "ElseClauseSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ForStatementSyntax.h"
#include "IfStatementSyntax.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

Parser::Parser(const SourceText &input): _input(input) {
    Lexer lexer(input);
    auto tokenList = lexer.tokenize();
    _tokens = std::vector(tokenList.begin(), tokenList.end()); // Convert list to vector
    const auto lex_diag = lexer.diagnostic_bag();
    _diagnostics = lex_diag;
    _position = 0;
}

SyntaxToken Parser::peek(const int offset) {
    if (_position + offset < _tokens.size())
        return _tokens[_position + offset];
    return _tokens[_tokens.size() - 1];
}

SyntaxToken Parser::current() {
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

ExpressionSyntax *Parser::parseExpression() {
    return parseAssignmentExpression();
}

StatementSyntax *Parser::parseIfStatement() {
    const auto keyword = match(SyntaxKind::IfKeyword);
    const auto condition = parseExpression();
    const auto statement = parseStatement();
    const auto elseClause = parseElseClause();
    return new IfStatementSyntax(keyword, *condition, *statement, elseClause);
}

StatementSyntax *Parser::parseWhileStatement() {
    const auto keyword = match(SyntaxKind::WhileKeyword);
    const auto condition = parseExpression();
    const auto body = parseStatement();
    return new WhileStatementSyntax(keyword, *condition, *body);
}

StatementSyntax *Parser::parseForStatement() {
    const auto keyword = match(SyntaxKind::ForKeyword);
    const auto identifier = match(SyntaxKind::IdentifierToken);
    const auto equalsToken = match(SyntaxKind::EqualsToken);
    const auto lowerBound = parseExpression();
    const auto toKeyword = match(SyntaxKind::ToKeyword);
    const auto upperBound = parseExpression();
    const auto body = parseStatement();
    return new ForStatementSyntax(keyword, identifier, equalsToken, *lowerBound, toKeyword, *upperBound, *body);
}

ElseClauseSyntax *Parser::parseElseClause() {
    if (current().kind != SyntaxKind::ElseKeyword)
        return nullptr;

    const auto keyword = nextToken();
    const auto statement = parseStatement();
    return new ElseClauseSyntax(keyword, *statement);
}

StatementSyntax *Parser::parseStatement() {
    switch (current().kind) {
        case SyntaxKind::OpenBraceToken:
            return parseBlockStatement();
        case SyntaxKind::LetKeyword:
        case SyntaxKind::VarKeyword:
            return parseVariableDeclaration();
        case SyntaxKind::IfKeyword:
            return parseIfStatement();
        case SyntaxKind::WhileKeyword:
            return parseWhileStatement();
        case SyntaxKind::ForKeyword:
            return parseForStatement();
        default:
            return parseExpressionStatement();
    }
}

BlockStatementSyntax *Parser::parseBlockStatement() {
    std::vector<StatementSyntax *> statements;

    const auto openBraceToken = match(SyntaxKind::OpenBraceToken);

    while (current().kind != SyntaxKind::EndOfFileToken &&
           current().kind != SyntaxKind::CloseBraceToken) {
        auto statement = parseStatement();
        statements.push_back(statement);
    }

    const auto closeBraceToken = match(SyntaxKind::CloseBraceToken);

    return new BlockStatementSyntax(openBraceToken, statements, closeBraceToken);
}

ExpressionStatementSyntax *Parser::parseExpressionStatement() {
    const auto expression = parseExpression();
    return new ExpressionStatementSyntax(*expression);
}

ExpressionSyntax *Parser::parseAssignmentExpression() {
    if (peek(0).kind == SyntaxKind::IdentifierToken &&
        peek(1).kind == SyntaxKind::EqualsToken) {
        const auto identifierToken = nextToken();
        const auto operatorToken = nextToken();
        const auto right = parseAssignmentExpression();
        return new AssignmentExpressionSyntax(identifierToken, operatorToken, *right);
    }

    return parseBinaryExpression();
}

ExpressionSyntax *Parser::parseBinaryExpression(const int parentPrecedence) {
    ExpressionSyntax *left;

    if (const int unaryOperatorPrecedence = SyntaxFacts::getUnaryPrecedence(current().kind);
        unaryOperatorPrecedence != 0 && unaryOperatorPrecedence >= parentPrecedence) {
        const SyntaxToken operatorToken = nextToken();
        auto *operand = parseBinaryExpression(unaryOperatorPrecedence);
        left = new UnaryExpressionSyntax(operatorToken, *operand);
    } else {
        left = parsePrimaryExpression();
    }

    while (true) {
        const int currentPrecedence = SyntaxFacts::getPrecedence(current().kind);
        if (currentPrecedence == 0 || currentPrecedence <= parentPrecedence)
            break;
        const auto operatorToken = nextToken();
        auto *right = parseBinaryExpression(currentPrecedence);
        left = new BinaryExpressionSyntax(*left, operatorToken, *right);
    }
    return left;
}

StatementSyntax *Parser::parseVariableDeclaration() {
    const auto crntToken = current();
    const auto expected = crntToken.kind == SyntaxKind::LetKeyword ? SyntaxKind::LetKeyword : SyntaxKind::VarKeyword;
    const auto keyword = match(expected);
    const auto identifier = match(SyntaxKind::IdentifierToken);
    auto equals = match(SyntaxKind::EqualsToken);
    const auto initializer = parseExpression();

    return new VariableDeclarationSyntax(crntToken, identifier, keyword, *initializer);
}

ExpressionSyntax *Parser::parsePrimaryExpression() {
    switch (current().kind) {
        case SyntaxKind::OpenParenthesisToken: {
            const auto left = nextToken();
            const auto expression = parseExpression();
            const auto right = match(SyntaxKind::CloseParenthesisToken);
            return new ParenthesizedExpressionSyntax(left, *expression, right);
        }
        case SyntaxKind::TrueKeyword:
        case SyntaxKind::FalseKeyword: {
            const auto keywordToken = nextToken();
            bool val = keywordToken.kind == SyntaxKind::TrueKeyword;
            return new LiteralExpressionSyntax(keywordToken, val);
        }
        case SyntaxKind::IdentifierToken: {
            const auto identifierToken = nextToken();
            return new NameExpressionSyntax(identifierToken);
        }
        default: {
            const auto numberToken = match(SyntaxKind::NumberToken);
            return new LiteralExpressionSyntax(numberToken);
        }
    }
}

CompilationUnitSyntax *Parser::ParseStatement() {
    const auto starement = parseStatement();
    const auto endOfFileToken = match(SyntaxKind::EndOfFileToken);
    return new CompilationUnitSyntax(*starement, endOfFileToken);
}


std::vector<SyntaxToken> Parser::getTokens(const std::string &input) {
    Lexer lexer(input);
    auto tokenList = lexer.tokenize();
    const auto tokens = std::vector(tokenList.begin(), tokenList.end()); // Convert list to vector
    return tokens;
}
