#include "SyntaxTree.h"
#include "Parser.h"
#include "Syntax.h"
#include "iostream"

SyntaxTree::SyntaxTree(DiagnosticBag *diagnostics,
                       ExpressionSyntax &root,
                       SyntaxToken endOfFileToken)
    : _diagnostics(diagnostics)
      , _root(root)
      , _endOfFileToken(std::move(endOfFileToken)) {
}

SyntaxTree::~SyntaxTree() = default;

 DiagnosticBag *SyntaxTree::diagnostics() const {
    return _diagnostics;
}

ExpressionSyntax &SyntaxTree::root() const{
    return _root;
}

const SyntaxToken &SyntaxTree::endOfFileToken() const {
    return _endOfFileToken;
}

SyntaxTree *SyntaxTree::parseToken(std::string text) {
    Parser parser(text);
    return parser.parse();
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

void SyntaxTree::prettyPrint(const SyntaxNode &node, std::string indent, const bool isLast) {
    // Using simple ASCII characters instead of UTF-8 box characters
    const std::string marker = isLast ? "+--" : "+--";
    std::cout << indent;
    std::cout << marker;
    std::cout << syntaxKindToString(node.getKind());

    if (auto *token = dynamic_cast<const SyntaxNodeToken *>(&node)) {

            std::cout << " " << token->getToken().text;


        // if (token->getKind() == SyntaxKind::NumberToken) {
        //     std::cout << " " << std::any_cast<int>(token->getToken().val);
        // }
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
