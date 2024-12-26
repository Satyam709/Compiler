#include "Compiler/SyntaxTree.h"
#include "Compiler/Parser.h"
#include "Compiler/Syntax.h"

SyntaxTree::SyntaxTree(std::vector<std::string_view> diagnostics,
                       ExpressionSyntax &root,
                       SyntaxToken endOfFileToken)
    : _diagnostics(std::move(diagnostics))
      , _root(root)
      , _endOfFileToken(std::move(endOfFileToken)) {
}

SyntaxTree::~SyntaxTree() = default;

const std::vector<std::string_view> SyntaxTree::diagnostics() const {
    return _diagnostics;
}

ExpressionSyntax& SyntaxTree::root() {
    return _root;
}

const SyntaxToken &SyntaxTree::endOfFileToken() const {
    return _endOfFileToken;
}

SyntaxTree* SyntaxTree::parseToken(std::string_view text) {
    Parser parser(text);
    return parser.parse();
}


SyntaxNodeToken::SyntaxNodeToken(const SyntaxToken &token)
    : _token(token) {
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
