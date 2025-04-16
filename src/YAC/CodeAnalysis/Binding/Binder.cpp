#include "Binder.h"

#include <stack>
#include <stack>

#include "BoundBlockStatement.h"
#include "BoundExpressionStatement.h"
#include "BoundGlobalScope.h"
#include "BoundScope.h"
#include "BoundVariableDeclaration.h"
#include "YAC/CodeAnalysis/Syntax/BlockStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/CompilationUnitSyntax.h"
#include "YAC/CodeAnalysis/Syntax/ExpressionStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/VariableDeclarationSyntax.h"

// BoundLiteralExpression Implementation
BoundLiteralExpression::BoundLiteralExpression(std::any value)
    : _value(std::move(value)) {
}

BoundNodeKind BoundLiteralExpression::getKind() const {
    return BoundNodeKind::LiteralExpression;
}

const std::type_info &BoundLiteralExpression::getType() const {
    return _value.type();
}

const std::any &BoundLiteralExpression::getValue() const {
    return _value;
}

// BoundBinaryExpression Implementation
BoundBinaryExpression::BoundBinaryExpression(const BoundExpression &left, const BoundBinaryOperator &op,
                                             const BoundExpression &right)
    : _left(left), _right(right), _operator(op) {
}

BoundNodeKind BoundBinaryExpression::getKind() const {
    return BoundNodeKind::BinaryExpression;
}

const BoundExpression &BoundBinaryExpression::left() const {
    return _left;
}

const BoundExpression &BoundBinaryExpression::right() const {
    return _right;
}

BoundBinaryOperatorKind BoundBinaryExpression::operator_() const {
    return _operator.kind1();
}

const std::type_info &BoundBinaryExpression::getType() const {
    return _operator.result_type();
}

BoundBinaryOperator BoundBinaryExpression::getOperator() const {
    return _operator;
}

// BoundUnaryExpression Implementation
BoundUnaryExpression::BoundUnaryExpression(const BoundUnaryOperator op, const BoundExpression &operand)
    : _op(op), _operand(operand) {
}

BoundNodeKind BoundUnaryExpression::getKind() const {
    return BoundNodeKind::UnaryExpression;
}

const std::type_info &BoundUnaryExpression::getType() const {
    return _op.result_type();
}

const BoundExpression *BoundUnaryExpression::getOperand() const {
    return &_operand;
}

BoundUnaryOperatorKind BoundUnaryExpression::getOperatorKind() const {
    return _op.kind1();
}

// BoundVariableExpression Implementation
BoundVariableExpression::BoundVariableExpression(const VariableSymbol &variable)
    : _variable(variable) {
}

BoundNodeKind BoundVariableExpression::getKind() const {
    return BoundNodeKind::VariableExpression;
}

const std::type_info &BoundVariableExpression::getType() const {
    return _variable.getType();
}

const std::string &BoundVariableExpression::getName() const {
    return _variable.getName();
}

const VariableSymbol &BoundVariableExpression::getVariable() const {
    return _variable;
}

// BoundAssignmentExpression Implementation
BoundAssignmentExpression::BoundAssignmentExpression(const VariableSymbol &variable, const BoundExpression *expression)
    : _variable(variable), _expression(expression) {
}

BoundNodeKind BoundAssignmentExpression::getKind() const {
    return BoundNodeKind::AssignmentExpression;
}

const std::type_info &BoundAssignmentExpression::getType() const {
    return _expression->getType();
}

const std::string &BoundAssignmentExpression::getName() const {
    return _variable.getName();
}

const BoundExpression *BoundAssignmentExpression::getExpression() const {
    return _expression;
}

const VariableSymbol &BoundAssignmentExpression::getVariable() const {
    return _variable;
}

// Binder Implementation
Binder::Binder(BoundScope *parent) : _diagnostic(new DiagnosticBag()), _scope(parent) {
}


DiagnosticBag *Binder::diagnostics() const {
    return _diagnostic;
}

BoundGlobalScope *Binder::BindGlobalScope(BoundGlobalScope *previous, CompilationUnitSyntax *syntax) {
    const auto parentScope = CreateParentScope(previous);
    const auto binder = new Binder(parentScope);
    auto expression = binder->bindStatement(&syntax->statement());
    const auto variables = binder->_scope->getDeclaredVariables();
    auto diagnostics = binder->diagnostics()->getDiagnostics();

    if (previous != nullptr) {
        auto previousDiagnostics = previous->diagnostics();
        diagnostics.insert(diagnostics.end(), previousDiagnostics.begin(), previousDiagnostics.end());
    }

    return new BoundGlobalScope(previous, diagnostics, variables, expression);
}

BoundScope *Binder::CreateParentScope(BoundGlobalScope *previous) {
    std::stack<BoundGlobalScope *> stack;

    while (previous != nullptr) {
        stack.push(previous);
        previous = previous->previous();
    }

    BoundScope *parent = nullptr;

    while (!stack.empty()) {
        previous = stack.top();
        stack.pop();
        const auto scope = new BoundScope(parent);
        for (auto var: previous->variables()) {
            scope->tryDeclare(var);
        }
        parent = scope;
    }
    if (parent == nullptr) {
        parent = new BoundScope(parent);
    }
    return parent;
}

const BoundExpression *Binder::BindLiteralExpression(const ExpressionSyntax &syntax) {
    if (const auto *exp = dynamic_cast<const LiteralExpressionSyntax *>(&syntax)) {
        try {
            std::any val = 0;
            val = exp->getToken().val;
            return new BoundLiteralExpression(val);
        } catch (const std::bad_any_cast &e) {
            std::cerr << "Cant bind to literal exp!!: " << std::endl;
            std::cerr << "Error: Failed to cast to int. Reason: " << e.what() << std::endl;
            return new BoundLiteralExpression(0);
        }
    }
    throw std::runtime_error("Failed to bind literal expression");
}

const BoundExpression *Binder::BindUnaryExpression(const ExpressionSyntax &syntax) {
    if (const auto *exp = dynamic_cast<const UnaryExpressionSyntax *>(&syntax)) {
        const auto boundOperand = bindExpression(*(exp->operand()));
        const auto boundOperator = BoundUnaryOperator::Bind(exp->operatorToken().kind, boundOperand->getType());
        if (boundOperator == nullptr) {
            _diagnostic->reportUndefinedUnaryOperator(
                exp->operatorToken().getSpan(),
                exp->operatorToken().text,
                getTypeName(boundOperand->getType())
            );
            return boundOperand;
        }
        return new BoundUnaryExpression(*boundOperator, *boundOperand);
    }
    throw std::invalid_argument("Invalid expression: expression failed to bound");
}

const BoundExpression *Binder::BindBinaryExpression(const ExpressionSyntax &syntax) {
    if (const auto *exp = dynamic_cast<const BinaryExpressionSyntax *>(&syntax)) {
        const auto left = bindExpression(exp->left());
        const auto right = bindExpression(exp->right());
        const auto op = BoundBinaryOperator::Bind(exp->operator_token().kind, left->getType(),
                                                  right->getType());

        if (op == nullptr) {
            _diagnostic->reportUndefinedBinaryOperator(
                exp->operator_token().getSpan(),
                exp->operator_token().text,
                getTypeName(left->getType()),
                getTypeName(right->getType())
            );
            return left;
        }

        return new BoundBinaryExpression(*left, *op, *right);
    }
    throw std::invalid_argument("Invalid expression: expression failed to bound");
}

const BoundExpression *Binder::BindNameExpression(const ExpressionSyntax &syntax) {
    if (const auto *exp = dynamic_cast<const NameExpressionSyntax *>(&syntax)) {
        std::string name = exp->getIdentifierToken().text;
        auto target = VariableSymbol(name, true, typeid(int));
        // just for searching as only name is hashed,type is of no use

        // if exists -> updates by reference '&target'
        if (!_scope->tryLookup(name, target)) {
            _diagnostic->reportUndefinedName(exp->getIdentifierToken().getSpan(), name);
            return new BoundLiteralExpression(0);
        }
        return new BoundVariableExpression(target);
    }
    throw std::invalid_argument("Invalid expression: expression failed to bound");
}

// ... existing code ...

BoundStatement *Binder::BindVariableDeclaration(VariableDeclarationSyntax *syntax) {
    const auto &name = syntax->getIdentifier().text;
    const bool isReadOnly = syntax->getKeyword().kind == SyntaxKind::LetKeyword;
    auto *initializer = bindExpression(syntax->getInitializer());
    auto *variable = new VariableSymbol(name, isReadOnly, initializer->getType());

    if (!_scope->tryDeclare(*variable)) {
        _diagnostic->reportVariableAlreadyDeclared(syntax->getIdentifier().getSpan(), name);
    }

    return new BoundVariableDeclaration(variable, initializer);
}

const BoundExpression *Binder::BindAssignmentExpression(const ExpressionSyntax &syntax) {
    if (const auto *exp = dynamic_cast<const AssignmentExpressionSyntax *>(&syntax)) {
        std::string name = exp->getIdentifierToken().text;
        const BoundExpression *boundExpression = bindExpression(exp->expression());
        const std::type_info &exprType = boundExpression->getType();

        // true is temporary
        auto crnt_var = VariableSymbol(name, true, exprType);

        if (exprType != typeid(int) && exprType != typeid(bool)) {
            throw std::runtime_error("Unsupported variable type: " + getTypeName(exprType));
        }
        std::any val = exprType == typeid(int) ? 0 : false;
        if (!_scope->tryLookup(name, crnt_var)) {
            _diagnostic->reportUndefinedName(exp->getIdentifierToken().getSpan(), name);
            return boundExpression;
        }
        if (crnt_var.isReadOnly()) {
            _diagnostic->reportCannotAssign(exp->getEqualsToken().getSpan(), name);
        }
        if (exprType != crnt_var.getType()) {
            _diagnostic->reportCannotConvert(exp->getSpan(), exprType, crnt_var.getType());
        }
        return new BoundAssignmentExpression(crnt_var, boundExpression);
    }
    throw std::invalid_argument("Invalid expression: expression failed to bound");
}

BoundStatement *Binder::bindStatement(StatementSyntax *syntax) {
    switch (syntax->getKind()) {
        case SyntaxKind::BlockStatement:
            return bindBlockStatement(static_cast<BlockStatementSyntax *>(syntax));
        case SyntaxKind::ExpressionStatement:
            return bindExpressionStatement(static_cast<ExpressionStatementSyntax *>(syntax));
        case SyntaxKind::VariableDeclaration:
            return BindVariableDeclaration(static_cast<VariableDeclarationSyntax *>(syntax));
        default:
            throw std::runtime_error("Unexpected syntax " + syntaxKindToString(syntax->getKind()));
    }
}

BoundStatement *Binder::bindBlockStatement(BlockStatementSyntax *syntax) {
    std::vector<BoundStatement *> statements;
    const auto parentScope = _scope;
    _scope = new BoundScope(parentScope);

    for (auto *statementSyntax: syntax->statements()) {
        auto *statement = bindStatement(statementSyntax);
        statements.push_back(statement);
    }

    _scope = _scope->parent();
    return new BoundBlockStatement(statements);
}

BoundStatement *Binder::bindExpressionStatement(ExpressionStatementSyntax *syntax) {
    auto *expression = bindExpression(syntax->expression());
    return new BoundExpressionStatement(expression);
}


const BoundExpression *Binder::bindExpression(const ExpressionSyntax &syntax) {
    switch (syntax.getKind()) {
        case SyntaxKind::LiteralExpression:
            return BindLiteralExpression(syntax);
        case SyntaxKind::UnaryExpression:
            return BindUnaryExpression(syntax);
        case SyntaxKind::BinaryExpression:
            return BindBinaryExpression(syntax);
        case SyntaxKind::ParenthesizedExpression: {
            if (const auto *exp = dynamic_cast<const ParenthesizedExpressionSyntax *>(&syntax)) {
                return bindExpression(exp->expression());
            }
        }
        case SyntaxKind::NameExpression:
            return BindNameExpression(syntax);
        case SyntaxKind::AssignmentExpression:
            return BindAssignmentExpression(syntax);
        default:
            throw std::runtime_error("Unknown syntax kind");
    }
}

// Helper Functions Implementations
std::string boundKindsToString(const BoundUnaryOperatorKind kind) {
    switch (kind) {
        case BoundUnaryOperatorKind::Invalid:
            return "Invalid";
        case BoundUnaryOperatorKind::Identity:
            return "Identity";
        case BoundUnaryOperatorKind::Negation:
            return "Negation";
        case BoundUnaryOperatorKind::LogicalAnd:
            return "LogicalAnd";
    }
    return "Not Found";
}

std::string boundKindsToString(const BoundBinaryOperatorKind kind) {
    switch (kind) {
        case BoundBinaryOperatorKind::Addition:
            return "Addition";
        case BoundBinaryOperatorKind::Subtraction:
            return "Subtraction";
        case BoundBinaryOperatorKind::Multiplication:
            return "Multiplication";
        case BoundBinaryOperatorKind::Division:
            return "Division";
        case BoundBinaryOperatorKind::LogicalAnd:
            return "LogicalAnd";
        case BoundBinaryOperatorKind::LogicalOR:
            return "LogicalOR";
        case BoundBinaryOperatorKind::Equals:
            return "Equals";
        case BoundBinaryOperatorKind::NotEquals:
            return "NotEquals";
        case BoundBinaryOperatorKind::Invalid:
            return "Invalid";
    }
    return "Not Found";
}

std::string boundKindsToString(const BoundNodeKind kind) {
    switch (kind) {
        case BoundNodeKind::BinaryExpression:
            return "BinaryExpression";
        case BoundNodeKind::UnaryExpression:
            return "UnaryExpression";
        case BoundNodeKind::LiteralExpression:
            return "LiteralExpression";
        case BoundNodeKind::VariableExpression:
            return "VariableExpression";
        case BoundNodeKind::AssignmentExpression:
            return "AssignmentExpression";
    }
    return "Not Found";
}
