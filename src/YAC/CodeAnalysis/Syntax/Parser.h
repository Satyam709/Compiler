#ifndef PARSER_H
#define PARSER_H
#include <vector>

#include "CompilationUnit.h"
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/CodeAnalysis/Text/SourceText.h"
class SyntaxTree;
class ExpressionSyntax;

class Parser {
public:
    explicit Parser(const SourceText& input);

    CompilationUnit *parse();

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
    ExpressionSyntax *parseAssignmentExpression();
    ExpressionSyntax *parseBinaryExpression(int parentPrecedence = 0);
    ExpressionSyntax *parsePrimaryExpression();

public:
    [[nodiscard]] DiagnosticBag * diagnostics() const {
        return _diagnostics;
    }
};
#endif //PARSER_H
