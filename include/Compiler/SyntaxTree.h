#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <string_view>
#include <vector>
#include "Syntax.h"

class SyntaxNode;

class ExpressionSyntax;

class SyntaxTree final {
public:
    SyntaxTree(std::vector<std::string_view> diagnostics, ExpressionSyntax &root, SyntaxToken endOfFileToken);

    ~SyntaxTree();

    const std::vector<std::string_view> diagnostics() const;

    ExpressionSyntax &root();

    const SyntaxToken &endOfFileToken() const;

    static SyntaxTree* parseToken(std::string_view text);

    static void prettyPrint(SyntaxNode &node, std::string indent = "", bool isLast = false);

private:
    std::vector<std::string_view> _diagnostics;
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

    const std::vector<SyntaxNode *> &getChildren() const override;

private:
    SyntaxToken _token;
};

#endif //SYNTAXTREE_H
