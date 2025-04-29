#include "BoundNode.h"

#include <sstream>
#include <stdexcept>  // For std::runtime_error

#include "BoundBlockStatement.h"
#include "BoundExpressionStatement.h"
#include "BoundForStatement.h"
#include "BoundIfStatement.h"
#include "BoundVariableDeclaration.h"
#include "BoundWhileStatement.h"

class VariableSymbol;


void BoundNode::writeTo(std::ostream& writer) const {
    prettyPrint(writer, this);
}

std::string BoundNode::toString() const {
    std::stringstream ss;
    writeTo(ss);
    return ss.str();
}

void BoundNode::prettyPrint(std::ostream& writer,
                           const BoundNode* node,
                           const std::string& indent,
                           bool isLast) {
    const std::string marker = isLast ? "+--" : "+--";
    writer << indent << marker << getText(node);

    // Print properties
    const auto& properties = node->getProperties();
    for(size_t i = 0; i < properties.size(); ++i) {
        if(i == 0) writer << " ";
        else writer << ", ";
        writer << properties[i].first << " = " << properties[i].second;
    }
    writer << "\n";

    // Recursively print children
    const std::string newIndent = indent + (isLast ? "   " : "│  ");
    const auto children = node->getChildren();
    if(!children.empty()) {
        const auto lastChild = children.back();
        for(const auto& child : children) {
            prettyPrint(writer, child, newIndent, child == lastChild);
        }
    }
}

std::string BoundNode::getText(const BoundNode* node) {
    if(auto b = dynamic_cast<const BoundBinaryExpression*>(node)) {
        return boundKindsToString(b->operator_()) + "Expression";
    }
    if(auto u = dynamic_cast<const BoundUnaryExpression*>(node)) {
        return boundKindsToString(u->getOperatorKind()) + "Expression";
    }
    return boundKindsToString(node->getKind());
}

std::string BoundNode::getColor(const BoundNode* node) {
    // ANSI escape codes for colors
    const std::string COLOR_RESET = "\033[0m";

    if (dynamic_cast<const BoundLiteralExpression*>(node))
        return "\033[35m";  // Magenta for literals

    if (dynamic_cast<const BoundVariableExpression*>(node))
        return "\033[36m";  // Cyan for variables

    if (dynamic_cast<const BoundBinaryExpression*>(node))
        return "\033[33m";  // Yellow for binary operators

    if (dynamic_cast<const BoundUnaryExpression*>(node))
        return "\033[93m";  // Bright yellow for unary operators

    if (dynamic_cast<const BoundAssignmentExpression*>(node))
        return "\033[32m";  // Green for assignments

    if (dynamic_cast<const BoundBlockStatement*>(node))
        return "\033[34m";  // Blue for block statements

    if (dynamic_cast<const BoundIfStatement*>(node) ||
        dynamic_cast<const BoundWhileStatement*>(node) ||
        dynamic_cast<const BoundForStatement*>(node))
        return "\033[94m";  // Bright blue for control flow statements

    if (dynamic_cast<const BoundExpressionStatement*>(node))
        return "\033[96m";  // Bright cyan for expression statements

    if (dynamic_cast<const BoundVariableDeclaration*>(node))
        return "\033[95m";  // Bright magenta for declarations

    if (dynamic_cast<const BoundStatement*>(node))
        return "\033[90m";  // Gray for generic statements

    if (dynamic_cast<const BoundExpression*>(node))
        return "\033[37m";  // White for generic expressions

    return "\033[31m";        // Red for unknown/unhandled nodes
}

// =============================================
// BoundLiteralExpression Implementation
// =============================================
BoundLiteralExpression::BoundLiteralExpression(std::any value)
    : _value(std::move(value)) {}

BoundNodeKind BoundLiteralExpression::getKind() const {
    return BoundNodeKind::LiteralExpression;
}

const std::type_info& BoundLiteralExpression::getType() const {
    return _value.type();
}

const std::any& BoundLiteralExpression::getValue() const {
    return _value;
}

std::vector<const BoundNode*> BoundLiteralExpression::getChildren() const {
    return {};
}

// In BoundNode.cpp
std::vector<std::pair<std::string, std::string>>
BoundLiteralExpression::getProperties() const {
    std::string valueStr;

    if(_value.type() == typeid(int)) {
        valueStr = std::to_string(std::any_cast<int>(_value));
    }
    else if(_value.type() == typeid(bool)) {
        valueStr = std::any_cast<bool>(_value) ? "true" : "false";
    }
    else {
        valueStr = "?";
    }

    return {
            {"Value", valueStr},
            {"Type", getType().name()}
    };
}
// =============================================
// BoundBinaryExpression Implementation
// =============================================
BoundBinaryExpression::BoundBinaryExpression(const BoundExpression& left,
                                            const BoundBinaryOperator& op,
                                            const BoundExpression& right)
    : _left(left), _right(right), _operator(op) {}

BoundNodeKind BoundBinaryExpression::getKind() const {
    return BoundNodeKind::BinaryExpression;
}

const BoundExpression& BoundBinaryExpression::left() const {
    return _left;
}

const BoundExpression& BoundBinaryExpression::right() const {
    return _right;
}

BoundBinaryOperatorKind BoundBinaryExpression::operator_() const {
    return _operator.kind1();
}

const std::type_info& BoundBinaryExpression::getType() const {
    return _operator.result_type();
}

BoundBinaryOperator BoundBinaryExpression::getOperator() const {
    return _operator;
}

std::vector<const BoundNode*> BoundBinaryExpression::getChildren() const {
    return { &_left, &_right };
}

std::vector<std::pair<std::string, std::string>> BoundBinaryExpression::getProperties() const {
    return {
            {"Operator", boundKindsToString(_operator.kind1())},
            {"ResultType", _operator.result_type().name()}
    };
}

// =============================================
// BoundUnaryExpression Implementation
// =============================================
BoundUnaryExpression::BoundUnaryExpression(const BoundUnaryOperator op,
                                          const BoundExpression& operand)
    : _op(op), _operand(operand) {}

BoundNodeKind BoundUnaryExpression::getKind() const {
    return BoundNodeKind::UnaryExpression;
}

const std::type_info& BoundUnaryExpression::getType() const {
    return _op.result_type();
}

const BoundExpression* BoundUnaryExpression::getOperand() const {
    return &_operand;
}

BoundUnaryOperatorKind BoundUnaryExpression::getOperatorKind() const {
    return _op.kind1();
}

std::vector<const BoundNode*> BoundUnaryExpression::getChildren() const {
    return { &_operand };
}

std::vector<std::pair<std::string, std::string>> BoundUnaryExpression::getProperties() const {
    return {
            {"Operator", boundKindsToString(_op.kind1())},
            {"ResultType", _op.result_type().name()}
    };
}

// =============================================
// BoundVariableExpression Implementation
// =============================================
BoundVariableExpression::BoundVariableExpression(const VariableSymbol& variable)
    : _variable(variable) {}

BoundNodeKind BoundVariableExpression::getKind() const {
    return BoundNodeKind::VariableExpression;
}

const std::type_info& BoundVariableExpression::getType() const {
    return _variable.getType();
}

const std::string& BoundVariableExpression::getName() const {
    return _variable.getName();
}

const VariableSymbol& BoundVariableExpression::getVariable() const {
    return _variable;
}

std::vector<const BoundNode*> BoundVariableExpression::getChildren() const {
    return {};
}

std::vector<std::pair<std::string, std::string>> BoundVariableExpression::getProperties() const {
    return {
            {"Name", _variable.getName()},
            {"Type", _variable.getType().name()}
    };
}

// =============================================
// BoundAssignmentExpression Implementation
// =============================================
BoundAssignmentExpression::BoundAssignmentExpression(const VariableSymbol& variable,
                                                    const BoundExpression* expression)
    : _variable(variable), _expression(expression) {}

BoundNodeKind BoundAssignmentExpression::getKind() const {
    return BoundNodeKind::AssignmentExpression;
}

const std::type_info& BoundAssignmentExpression::getType() const {
    return _expression->getType();
}

const std::string& BoundAssignmentExpression::getName() const {
    return _variable.getName();
}

const BoundExpression* BoundAssignmentExpression::getExpression() const {
    return _expression;
}

const VariableSymbol& BoundAssignmentExpression::getVariable() const {
    return _variable;
}

std::vector<const BoundNode*> BoundAssignmentExpression::getChildren() const {
    return { _expression };
}

std::vector<std::pair<std::string, std::string>> BoundAssignmentExpression::getProperties() const {
    return {
            {"Variable", _variable.getName()},
            {"VariableType", _variable.getType().name()},
            {"ExpressionType", _expression->getType().name()}
    };
}

// =============================================
// Enum Conversion Helpers
// =============================================

std::string boundKindsToString(const BoundUnaryOperatorKind kind) {
    switch (kind) {
        case BoundUnaryOperatorKind::Identity: return "Identity";
        case BoundUnaryOperatorKind::Negation: return "Negation";
        case BoundUnaryOperatorKind::Invalid: return "Invalid";
        case BoundUnaryOperatorKind::LogicalAnd: return "LogicalAnd";
        case BoundUnaryOperatorKind::OnesComplement: return "OnesComplement";
        default: return "UnknownUnaryOperator";
    }
}

std::string boundKindsToString(const BoundBinaryOperatorKind kind) {
    switch (kind) {
        case BoundBinaryOperatorKind::Addition: return "Addition";
        case BoundBinaryOperatorKind::Subtraction: return "Subtraction";
        case BoundBinaryOperatorKind::Multiplication: return "Multiplication";
        case BoundBinaryOperatorKind::Division: return "Division";
        case BoundBinaryOperatorKind::Invalid: return "Invalid";
        case BoundBinaryOperatorKind::LogicalAnd: return "LogicalAnd";
        case BoundBinaryOperatorKind::LogicalOR: return "LogicalOR";
        case BoundBinaryOperatorKind::Equals: return "Equals";
        case BoundBinaryOperatorKind::NotEquals: return "NotEquals";
        case BoundBinaryOperatorKind::Less: return "Less";
        case BoundBinaryOperatorKind::LessOrEquals: return "LessOrEquals";
        case BoundBinaryOperatorKind::Greater: return "Greater";
        case BoundBinaryOperatorKind::BitwiseAnd: return "BitwiseAnd";
        case BoundBinaryOperatorKind::BitwiseOr: return "BitwiseOr";
        case BoundBinaryOperatorKind::BitwiseXor: return "BitwiseXor";
        case BoundBinaryOperatorKind::GreaterOrEquals: return "GreaterOrEquals";
        default: return "UnknownBinaryOperator";
    }
}

std::string boundKindsToString(const BoundNodeKind kind) {
    switch (kind) {
        case BoundNodeKind::UnaryExpression: return "UnaryExpression";
        case BoundNodeKind::LiteralExpression: return "LiteralExpression";
        case BoundNodeKind::BinaryExpression: return "BinaryExpression";
        case BoundNodeKind::VariableExpression: return "VariableExpression";
        case BoundNodeKind::AssignmentExpression: return "AssignmentExpression";
        case BoundNodeKind::BlockStatement: return "BlockStatement";
        case BoundNodeKind::ExpressionStatement: return "ExpressionStatement";
        case BoundNodeKind::VariableDeclaration: return "VariableDeclaration";
        case BoundNodeKind::IfStatement: return "IfStatement";
        case BoundNodeKind::WhileStatement: return "WhileStatement";
        case BoundNodeKind::ForStatement: return "ForStatement";
        default: return "UnknownNodeKind";
    }
}