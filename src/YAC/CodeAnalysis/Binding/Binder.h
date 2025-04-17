#ifndef BINDER_H
#define BINDER_H

#include <any>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <typeinfo>
#include <memory>

#include "BoundGlobalScope.h"
#include "BoundOperators.h"
#include "../Syntax/Expression.h"
#include "../Syntax/Syntax.h"
#include "../../Utils/Caster.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "unordered_map"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"
#include "YAC/CodeAnalysis/Syntax/BlockStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/ExpressionStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/IfStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/VariableDeclarationSyntax.h"
#include "YAC/CodeAnalysis/Syntax/WhileStatementSyntax.h"

class BoundScope;

enum class BoundNodeKind {
    UnaryExpression,
    LiteralExpression,
    BinaryExpression,
    VariableExpression,
    AssignmentExpression,
    BlockStatement,
    ExpressionStatement,
    VariableDeclaration,
    IfStatement,
    WhileStatement
};

class BoundNode {
public:
    virtual BoundNodeKind getKind() const = 0;
};

class BoundExpression : public BoundNode {
public:
    virtual const std::type_info &getType() const = 0;
};

class BoundLiteralExpression : public BoundExpression {
public:
    explicit BoundLiteralExpression(std::any value);

    BoundNodeKind getKind() const override;

    const std::type_info &getType() const override;

    const std::any &getValue() const;

private:
    std::any _value;
};

enum class BoundUnaryOperatorKind {
    Identity,
    Negation,
    Invalid,
    LogicalAnd,
};

enum class BoundBinaryOperatorKind {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Invalid,
    LogicalAnd,
    LogicalOR,
    Equals,
    NotEquals,
    Less,
    LessOrEquals,
    Greater,
    GreaterOrEquals
};

class BoundBinaryExpression : public BoundExpression {
public:
    BoundBinaryExpression(const BoundExpression &left, const BoundBinaryOperator &op,
                          const BoundExpression &right);

    BoundNodeKind getKind() const override;

    const BoundExpression &left() const;

    const BoundExpression &right() const;

    BoundBinaryOperatorKind operator_() const;

    const std::type_info &getType() const override;

    BoundBinaryOperator getOperator() const;

private:
    const BoundExpression &_left;
    const BoundExpression &_right;
    BoundBinaryOperator _operator;
};

class BoundUnaryExpression : public BoundExpression {
public:
    BoundUnaryExpression(const BoundUnaryOperator op, const BoundExpression &operand);

    BoundNodeKind getKind() const override;

    const std::type_info &getType() const override;

    const BoundExpression *getOperand() const;

    BoundUnaryOperatorKind getOperatorKind() const;

private:
    BoundUnaryOperator _op;
    const BoundExpression &_operand;
};

class BoundVariableExpression : public BoundExpression {
public:
    explicit BoundVariableExpression(const VariableSymbol &variable);

    BoundNodeKind getKind() const override;

    const std::type_info &getType() const override;

    const std::string &getName() const;

    const VariableSymbol &getVariable() const;

private:
    VariableSymbol _variable;
};

class BoundAssignmentExpression : public BoundExpression {
public:
    BoundAssignmentExpression(const VariableSymbol &variable, const BoundExpression *expression);

    BoundNodeKind getKind() const override;

    const std::type_info &getType() const override;

    const std::string &getName() const;

    const BoundExpression *getExpression() const;

    const VariableSymbol &getVariable() const;

private:
    VariableSymbol _variable;
    const BoundExpression *_expression;
};

class Binder {
private:
    DiagnosticBag *_diagnostic;
    BoundScope *_scope;

public:
    explicit Binder(BoundScope *parent);

    DiagnosticBag *diagnostics() const;

    static BoundGlobalScope *BindGlobalScope(BoundGlobalScope *previous, const CompilationUnitSyntax *compilation_unit);

    static BoundScope *CreateParentScope(BoundGlobalScope *previous);

    const BoundExpression *BindLiteralExpression(const ExpressionSyntax &syntax);

    const BoundExpression *BindUnaryExpression(const ExpressionSyntax &syntax);

    const BoundExpression *BindBinaryExpression(const ExpressionSyntax &syntax);

    const BoundExpression *BindNameExpression(const ExpressionSyntax &syntax);

    BoundStatement *BindVariableDeclaration(const VariableDeclarationSyntax *syntax);

    const BoundExpression *BindAssignmentExpression(const ExpressionSyntax &syntax);

    const BoundExpression *bindExpression(const ExpressionSyntax &syntax, const std::type_info &targetType);

    BoundStatement *bindIfStatement(const IfStatementSyntax *syntax);

    BoundStatement *bindWhileStatement(const WhileStatementSyntax *syntax);

    BoundStatement *bindStatement(StatementSyntax *syntax);

    BoundStatement *bindBlockStatement(const BlockStatementSyntax *syntax);

    BoundStatement *bindExpressionStatement(const ExpressionStatementSyntax *syntax);

    const BoundExpression *bindExpression(const ExpressionSyntax &syntax);
};

std::string boundKindsToString(const BoundUnaryOperatorKind kind);

std::string boundKindsToString(const BoundBinaryOperatorKind kind);

std::string boundKindsToString(const BoundNodeKind kind);

#endif // BINDER_H
