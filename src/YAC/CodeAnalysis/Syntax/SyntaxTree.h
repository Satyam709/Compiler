#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <string>
#include <vector>
#include <YAC/CodeAnalysis/Text/SourceText.h>

#include "Syntax.h"
#include "YAC/CodeAnalysis/Diagnostics.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/CodeAnalysis/Text/TextSpan.h"

class SyntaxNode;

class ExpressionSyntax;

class SyntaxTree final {
public:
    SyntaxTree(const SourceText& text,DiagnosticBag *diagnostics, ExpressionSyntax &root, SyntaxToken endOfFileToken);

    ~SyntaxTree();

    const SourceText& text() const { return _text; }

    DiagnosticBag *diagnostics() const;

    ExpressionSyntax &root() const;

    const SyntaxToken &endOfFileToken() const;

    static SyntaxTree* parse(const SourceText& text);


    static void prettyPrint(const SyntaxNode &node, std::string indent = "", bool isLast = false);

private:
    const SourceText _text;
    DiagnosticBag *_diagnostics;
    ExpressionSyntax &_root;
    SyntaxToken _endOfFileToken;
};

// syntax node interface
class SyntaxNode {
public:
    virtual SyntaxKind getKind() const = 0;

    virtual const std::vector<SyntaxNode*>& getChildren() const = 0;

    // Add virtual Span method
    virtual TextSpan getSpan() const {
        const auto& children = getChildren();
        if (children.empty()) {
            return TextSpan(0, 0); // Or handle empty case as needed
        }

        TextSpan firstSpan = children.front()->getSpan();
        TextSpan lastSpan = children.back()->getSpan();
        return TextSpan::FromBounds(firstSpan.Start(), lastSpan.End());
    }

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
