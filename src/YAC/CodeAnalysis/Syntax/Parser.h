#ifndef PARSER_H
#define PARSER_H
#include <vector>

#include "BlockStatementSyntax.h"
#include "CompilationUnitSyntax.h"
#include "ElseClauseSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/CodeAnalysis/Text/SourceText.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const SourceText& input);

    CompilationUnitSyntax *ParseStatement();

    static std::vector<SyntaxToken> getTokens(const std::string &input);

private:
    std::vector<SyntaxToken> _tokens;
    DiagnosticBag *_diagnostics;
    int _position;
    SourceText _input;

    SyntaxToken peek(int offset);
    SyntaxToken current();
    SyntaxToken nextToken();
    SyntaxToken match(SyntaxKind kind);

    ExpressionSyntax *parseExpression();

    StatementSyntax *parseIfStatement();

    StatementSyntax *parseWhileStatement();

    ElseClauseSyntax *parseElseClause();

    StatementSyntax *parseStatement();

    BlockStatementSyntax *parseBlockStatement();

    ExpressionStatementSyntax *parseExpressionStatement();

    ExpressionSyntax *parseAssignmentExpression();
    ExpressionSyntax *parseBinaryExpression(int parentPrecedence = 0);

    StatementSyntax *parseVariableDeclaration();

    ExpressionSyntax *parsePrimaryExpression();

public:
    [[nodiscard]] DiagnosticBag * diagnostics() const {
        return _diagnostics;
    }
};
#endif //PARSER_H
