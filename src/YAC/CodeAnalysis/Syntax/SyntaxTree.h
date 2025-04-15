#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include <string>
#include <vector>
#include <YAC/CodeAnalysis/Text/SourceText.h>
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/CodeAnalysis/Text/TextSpan.h"

class CompilationUnit;
class SyntaxNode;
class ExpressionSyntax;

class SyntaxTree final {
public:
    explicit SyntaxTree(SourceText text);

    ~SyntaxTree();

    [[nodiscard]] const SourceText& text() const { return _text; }

    [[nodiscard]] DiagnosticBag *diagnostics() const;

    static SyntaxTree* parse(const SourceText& text);

    static void prettyPrint(const SyntaxNode &node, std::string indent = "", bool isLast = false);

    [[nodiscard]] CompilationUnit * root() const {
        return _root;
    }

private:
    const SourceText _text;
    DiagnosticBag *_diagnostics;
    CompilationUnit * _root;
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
