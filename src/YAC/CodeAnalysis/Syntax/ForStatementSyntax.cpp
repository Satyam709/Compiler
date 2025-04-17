//
// Created by satya on 17-04-2025.
//

#include "ForStatementSyntax.h"

ForStatementSyntax::ForStatementSyntax(const SyntaxToken &keyword,
                                       const SyntaxToken &identifier,
                                       const SyntaxToken &equalsToken,
                                       ExpressionSyntax &lowerBound,
                                       const SyntaxToken &toKeyword,
                                       ExpressionSyntax &upperBound,
                                       StatementSyntax &body)
    : _keyword(keyword)
      , _identifier(identifier)
      , _equalsToken(equalsToken)
      , _lowerBound(lowerBound)
      , _toKeyword(toKeyword)
      , _upperBound(upperBound)
      , _body(body) {
    _children.push_back(new SyntaxNodeToken(keyword));
    _children.push_back(new SyntaxNodeToken(identifier));
    _children.push_back(new SyntaxNodeToken(equalsToken));
    _children.push_back(&lowerBound);
    _children.push_back(new SyntaxNodeToken(toKeyword));
    _children.push_back(&upperBound);
    _children.push_back(&body);
}

SyntaxKind ForStatementSyntax::getKind() const {
    return SyntaxKind::ForStatement;
}

const std::vector<SyntaxNode *> &ForStatementSyntax::getChildren() const {
    return _children;
}
