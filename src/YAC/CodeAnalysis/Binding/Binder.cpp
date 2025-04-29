#include "Binder.h"

#include <stack>
#include <stack>

#include "BoundBlockStatement.h"
#include "BoundExpressionStatement.h"
#include "BoundForStatement.h"
#include "BoundGlobalScope.h"
#include "BoundIfStatement.h"
#include "BoundScope.h"
#include "BoundVariableDeclaration.h"
#include "BoundWhileStatement.h"
#include "YAC/CodeAnalysis/Syntax/BlockStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/CompilationUnitSyntax.h"
#include "YAC/CodeAnalysis/Syntax/ExpressionStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/ForStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/IfStatementSyntax.h"
#include "YAC/CodeAnalysis/Syntax/VariableDeclarationSyntax.h"
#include "YAC/CodeAnalysis/Syntax/WhileStatementSyntax.h"

// Binder Implementation
Binder::Binder(BoundScope *parent) : _diagnostic(new DiagnosticBag()), _scope(parent) {
}


DiagnosticBag *Binder::diagnostics() const {
    return _diagnostic;
}

BoundGlobalScope *Binder::BindGlobalScope(BoundGlobalScope *previous, const CompilationUnitSyntax *syntax) {
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

        if (name.empty())
        {
            // This means the token was inserted by the parser. We already
            // reported error so we can just return an error expression.
            return new BoundLiteralExpression(0);
        }

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

BoundStatement *Binder::BindVariableDeclaration(const VariableDeclarationSyntax *syntax) {
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
            _diagnostic->reportCannotConvert(exp->expression().getSpan(), exprType, crnt_var.getType());
        }
        return new BoundAssignmentExpression(crnt_var, boundExpression);
    }
    throw std::invalid_argument("Invalid expression: expression failed to bound");
}

const BoundExpression *Binder::bindExpression(const ExpressionSyntax &syntax, const std::type_info &targetType) {
    const auto *result = bindExpression(syntax);
    if (result->getType() != targetType) {
        _diagnostic->reportCannotConvert(syntax.getSpan(), result->getType(), targetType);
    }
    return result;
}

BoundStatement *Binder::bindIfStatement(const IfStatementSyntax *syntax) {
    const auto *condition = bindExpression(syntax->condition(), typeid(bool));
    const auto *thenStatement = bindStatement(&syntax->thenStatement());
    const auto *elseStatement = syntax->elseClause() ? bindStatement(&syntax->elseClause()->elseStatement()) : nullptr;
    return new BoundIfStatement(condition, thenStatement, elseStatement);
}

BoundStatement *Binder::bindWhileStatement(const WhileStatementSyntax *syntax) {
    const auto condition = bindExpression(syntax->condition(), typeid(bool));
    const auto body = bindStatement(&syntax->body());
    return new BoundWhileStatement(condition, body);
}

BoundStatement *Binder::bindForStatement(const ForStatementSyntax *syntax) {
    const auto lowerBound = bindExpression(syntax->lowerBound(), typeid(int));
    const auto upperBound = bindExpression(syntax->upperBound(), typeid(int));

    _scope = new BoundScope(_scope);

    const auto &name = syntax->identifier().text;
    auto variable = VariableSymbol(name, true, typeid(int));
    if (!_scope->tryDeclare(variable)) {
        _diagnostic->reportVariableAlreadyDeclared(syntax->identifier().getSpan(), name);
    }

    const auto body = bindStatement(&syntax->body());

    _scope = _scope->parent();

    return new BoundForStatement(variable, lowerBound, upperBound, body);
}


BoundStatement *Binder::bindStatement(StatementSyntax *syntax) {
    switch (syntax->getKind()) {
        case SyntaxKind::BlockStatement:
            return bindBlockStatement(static_cast<BlockStatementSyntax *>(syntax));
        case SyntaxKind::ExpressionStatement:
            return bindExpressionStatement(static_cast<ExpressionStatementSyntax *>(syntax));
        case SyntaxKind::VariableDeclaration:
            return BindVariableDeclaration(static_cast<VariableDeclarationSyntax *>(syntax));
        case SyntaxKind::IfStatement:
            return bindIfStatement(static_cast<IfStatementSyntax *>(syntax));
        case SyntaxKind::WhileStatement:
            return bindWhileStatement(dynamic_cast<WhileStatementSyntax *>(syntax));
        case SyntaxKind::ForStatement:
            return bindForStatement(dynamic_cast<ForStatementSyntax *>(syntax));
        default:
            throw std::runtime_error("Unexpected syntax " + syntaxKindToString(syntax->getKind()));
    }
}

BoundStatement *Binder::bindBlockStatement(const BlockStatementSyntax *syntax) {
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

BoundStatement *Binder::bindExpressionStatement(const ExpressionStatementSyntax *syntax) {
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


