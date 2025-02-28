#ifndef BINDER_H
#define BINDER_H

#include <any>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <typeinfo>
#include <memory>

#include "BoundOperators.h"
#include "../Syntax/Expression.h"
#include "../Syntax/Syntax.h"
#include "../../Utils/Caster.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "unordered_map"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"

enum class BoundNodeKind {
    UnaryExpression,
    LiteralExpression,
    BinaryExpression,
    VariableExpression,
    AssignmentExpression
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
    NotEquals
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
    explicit BoundVariableExpression(const VariableSymbol& variable);

    BoundNodeKind getKind() const override;
    const std::type_info& getType() const override;
    const std::string& getName() const;
    const VariableSymbol& getVariable() const;

private:
    VariableSymbol _variable;
};

class BoundAssignmentExpression : public BoundExpression {
public:
    BoundAssignmentExpression(const VariableSymbol& variable, const BoundExpression* expression);

    BoundNodeKind getKind() const override;
    const std::type_info& getType() const override;
    const std::string& getName() const;
    const BoundExpression* getExpression() const;
    const VariableSymbol& getVariable() const;

private:
    VariableSymbol _variable;
    const BoundExpression* _expression;
};

class Binder {
private:
    DiagnosticBag* _diagnostic;
    std::unordered_map<VariableSymbol, std::any>& _variables;

public:
    explicit Binder(std::unordered_map<VariableSymbol, std::any>& variables);

    DiagnosticBag *diagnostics() const;
    const BoundExpression *BindLiteralExpression(const ExpressionSyntax &syntax);
    const BoundExpression *BindUnaryExpression(const ExpressionSyntax &syntax);
    const BoundExpression *BindBinaryExpression(const ExpressionSyntax &syntax);
    const BoundExpression* BindNameExpression(const ExpressionSyntax& syntax);
    const BoundExpression* BindAssignmentExpression(const ExpressionSyntax& syntax);
    const BoundExpression *bindExpression(const ExpressionSyntax &syntax);
};

std::string boundKindsToString(const BoundUnaryOperatorKind kind);
std::string boundKindsToString(const BoundBinaryOperatorKind kind);
std::string boundKindsToString(const BoundNodeKind kind);

#endif // BINDER_H