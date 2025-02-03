#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <string>
#include <vector>
#include "Syntax.h"
#include "YAC/CodeAnalysis/Diagnostics.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"

class SyntaxNode;

class ExpressionSyntax;

class SyntaxTree final {
public:
    SyntaxTree(DiagnosticBag *diagnostics, ExpressionSyntax &root, SyntaxToken endOfFileToken);

    ~SyntaxTree();

    DiagnosticBag *diagnostics() const;

    const ExpressionSyntax &root() const;

    const SyntaxToken &endOfFileToken() const;

    static SyntaxTree* parseToken(std::string text);

    static void prettyPrint(const SyntaxNode &node, std::string indent = "", bool isLast = false);

private:
    DiagnosticBag *_diagnostics;
    ExpressionSyntax &_root;
    SyntaxToken _endOfFileToken;
};

// syntax node interface
class SyntaxNode {
public:
    virtual SyntaxKind getKind() const = 0;

    virtual const std::vector<SyntaxNode *> &getChildren() const = 0;

    virtual ~SyntaxNode() = default;
};

// a wrapper of syntax token on syntax node -> represents leaf node(terminals)
class SyntaxNodeToken final : public SyntaxNode {
public:
    explicit SyntaxNodeToken(const SyntaxToken &token);

    SyntaxKind getKind() const override;

    const SyntaxToken &getToken() const;

    const std::any &getValue() const;

    const std::vector<SyntaxNode *> &getChildren() const override;

private:
    SyntaxToken _token;
};

#endif //SYNTAXTREE_H
