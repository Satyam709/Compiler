#include "SyntaxTree.h"

#include <utility>
#include "Parser.h"
#include "Syntax.h"
#include "iostream"

// New constructor with SourceText
SyntaxTree::SyntaxTree(SourceText text)
    : _text(std::move(text)) {
    Parser parser(_text);
    _root = parser.ParseStatement();
    _diagnostics = parser.diagnostics();
}

SyntaxTree::~SyntaxTree() = default;

DiagnosticBag *SyntaxTree::diagnostics() const {
    return _diagnostics;
}

SyntaxTree *SyntaxTree::parse(const SourceText &text) {
    return new SyntaxTree(text);
}

SyntaxNodeToken::SyntaxNodeToken(const SyntaxToken &token)
    : _token(token) {
}

const std::any &SyntaxNodeToken::getValue() const {
    return _token.val;
}

SyntaxKind SyntaxNodeToken::getKind() const {
    return _token.kind;
}

const SyntaxToken &SyntaxNodeToken::getToken() const {
    return _token;
}

const std::vector<SyntaxNode *> &SyntaxNodeToken::getChildren() const {
    static const std::vector<SyntaxNode *> emptyChildren;
    return emptyChildren;
}

TextSpan SyntaxNodeToken::getSpan() const {
    return _token.getSpan();
}

void SyntaxTree::prettyPrint(const SyntaxNode &node, std::string indent, const bool isLast) {
    // Using simple ASCII characters instead of UTF-8 box characters
    const std::string marker = isLast ? "+--" : "+--";
    std::cout << indent;
    std::cout << marker;
    std::cout << syntaxKindToString(node.getKind());

    if (auto *token = dynamic_cast<const SyntaxNodeToken *>(&node)) {
        std::cout << " " << token->getToken().text;
    }
    std::cout << std::endl;

    // Using simple ASCII vertical line
    indent += isLast ? "    " : "|   ";

    if (const auto &children = node.getChildren(); !children.empty()) {
        for (size_t i = 0; i < children.size(); ++i) {
            prettyPrint(*children[i], indent, i == children.size() - 1);
        }
    }
}
