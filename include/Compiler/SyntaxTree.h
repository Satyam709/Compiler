#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <string_view>
#include <vector>
#include "Syntax.h"

class ExpressionSyntax;

class SyntaxTree final {
public:
    // constructor
    SyntaxTree(std::vector<std::string_view> diagnostics, ExpressionSyntax &root, SyntaxToken endOfFileToken);

    // default destructor
    ~SyntaxTree();

    // returns diagnostics
    const std::vector<std::string_view> diagnostics() const;

    // returns SyntaxTree root
    ExpressionSyntax &root();

    // returns end of file token
    const SyntaxToken &endOfFileToken() const;

    
    static SyntaxTree* parseToken(std::string_view text);

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
    // constructor
    explicit SyntaxNodeToken(const SyntaxToken &token);

    // returns kind of token
    SyntaxKind getKind() const override;

    // getter function for token
    const SyntaxToken &getToken() const;

    
    const std::vector<SyntaxNode *> &getChildren() const override;

private:
    SyntaxToken _token;
};

#endif //SYNTAXTREE_H
