#ifndef BOUND_NODE_H
#define BOUND_NODE_H

#include <any>
#include <typeinfo>
#include <string>
#include "BoundOperators.h"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"

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
    WhileStatement,
    ForStatement
};

enum class BoundUnaryOperatorKind {
    Identity,
    Negation,
    Invalid,
    LogicalAnd,
    OnesComplement,
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
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    GreaterOrEquals
};

class BoundNode {
public:
    virtual ~BoundNode() = default;
    virtual BoundNodeKind getKind() const = 0;
};

class BoundExpression : public BoundNode {
public:
    virtual const std::type_info& getType() const = 0;
};

class BoundLiteralExpression : public BoundExpression {
public:
    explicit BoundLiteralExpression(std::any value);
    BoundNodeKind getKind() const override;
    const std::type_info& getType() const override;
    const std::any& getValue() const;

private:
    std::any _value;
};

class BoundBinaryExpression : public BoundExpression {
public:
    BoundBinaryExpression(const BoundExpression& left, const BoundBinaryOperator& op, const BoundExpression& right);
    BoundNodeKind getKind() const override;
    const BoundExpression& left() const;
    const BoundExpression& right() const;
    BoundBinaryOperatorKind operator_() const;
    const std::type_info& getType() const override;
    BoundBinaryOperator getOperator() const;

private:
    const BoundExpression& _left;
    const BoundExpression& _right;
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

// Helper functions for converting enums to strings
std::string boundKindsToString(BoundUnaryOperatorKind kind);
std::string boundKindsToString(BoundBinaryOperatorKind kind);
std::string boundKindsToString(BoundNodeKind kind);

#endif // BOUND_NODE_H