#ifndef BINDER_H
#define BINDER_H

#include "BoundNode.h"           // Replaces all bound node declarations
#include "BoundGlobalScope.h"

#include "BoundScope.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/CodeAnalysis/Syntax/StatementSyntax.h"

// Forward declarations for syntax classes
class CompilationUnitSyntax;
class BlockStatementSyntax;
class StatementSyntax;
class ExpressionStatementSyntax;
class ForStatementSyntax;
class IfStatementSyntax;
class VariableDeclarationSyntax;
class WhileStatementSyntax;
class SyntaxNode;

class Binder {
private:
    DiagnosticBag* _diagnostic;
    BoundScope* _scope;

public:
    explicit Binder(BoundScope* parent);
    DiagnosticBag* diagnostics() const;

    // Global scope binding
    static BoundGlobalScope* BindGlobalScope(
        BoundGlobalScope* previous,
        const CompilationUnitSyntax* compilation_unit
    );

    // Scope management
    static BoundScope* CreateParentScope(BoundGlobalScope* previous);

    // Expression binding
    const BoundExpression* BindLiteralExpression(const ExpressionSyntax& syntax);
    const BoundExpression* BindUnaryExpression(const ExpressionSyntax& syntax);
    const BoundExpression* BindBinaryExpression(const ExpressionSyntax& syntax);
    const BoundExpression* BindNameExpression(const ExpressionSyntax& syntax);
    const BoundExpression* BindAssignmentExpression(const ExpressionSyntax& syntax);
    const BoundExpression* bindExpression(const ExpressionSyntax& syntax, const std::type_info& targetType);
    const BoundExpression* bindExpression(const ExpressionSyntax& syntax);

    // Statement binding
    BoundStatement* BindVariableDeclaration(const VariableDeclarationSyntax* syntax);
    BoundStatement* bindIfStatement(const IfStatementSyntax* syntax);
    BoundStatement* bindWhileStatement(const WhileStatementSyntax* syntax);
    BoundStatement* bindForStatement(const ForStatementSyntax* syntax);
    BoundStatement* bindStatement(StatementSyntax* syntax);
    BoundStatement* bindBlockStatement(const BlockStatementSyntax* syntax);
    BoundStatement* bindExpressionStatement(const ExpressionStatementSyntax* syntax);
};

#endif // BINDER_H