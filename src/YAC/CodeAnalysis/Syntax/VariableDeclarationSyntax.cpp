//
// Created by satya on 17-04-2025.
//

#include "VariableDeclarationSyntax.h"

VariableDeclarationSyntax::VariableDeclarationSyntax(const SyntaxToken &keyword,
                                                     const SyntaxToken &identifier,
                                                     const SyntaxToken &equalsToken,
                                                     ExpressionSyntax &initializer)
    : _keyword(keyword)
      , _identifier(identifier)
      , _equalsToken(equalsToken)
      , _initializer(initializer) {
    _children.push_back(new SyntaxNodeToken(keyword));
    _children.push_back(new SyntaxNodeToken(identifier));
    _children.push_back(new SyntaxNodeToken(equalsToken));
    _children.push_back(&initializer);
}

SyntaxKind VariableDeclarationSyntax::getKind() const {
    return SyntaxKind::VariableDeclaration;
}

const std::vector<SyntaxNode *> &VariableDeclarationSyntax::getChildren() const {
    return _children;
}

const SyntaxToken &VariableDeclarationSyntax::getKeyword() const {
    return _keyword;
}

const SyntaxToken &VariableDeclarationSyntax::getIdentifier() const {
    return _identifier;
}

const SyntaxToken &VariableDeclarationSyntax::getEqualsToken() const {
    return _equalsToken;
}

ExpressionSyntax &VariableDeclarationSyntax::getInitializer() const {
    return _initializer;
}
