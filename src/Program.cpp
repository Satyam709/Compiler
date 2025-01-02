#include <iostream>
#include <string>
#include <vector>

#include "Compiler/Evaluator.h"
#include "Compiler/SyntaxTree.h"
#include "Compiler/Binder.h"

static void prettyPrint(const SyntaxNode &node, std::string indent = "", bool isLast = true) {
    // Using simple ASCII characters instead of UTF-8 box characters
    std::string marker = isLast ? "|__" : "|--";
    std::cout << indent;
    std::cout << marker;
    std::cout << syntaxKindToString(node.getKind());

    if (auto *token = dynamic_cast<const SyntaxNodeToken *>(&node)) {
        if (token->getKind()) {
            std::cout << " " << token->getToken().text;
        }
    }
    std::cout << std::endl;

    // Using simple ASCII vertical line
    indent += isLast ? "   " : "|   ";

    const auto &children = node.getChildren();
    if (!children.empty()) {
        for (size_t i = 0; i < children.size(); ++i) {
            prettyPrint(*children[i], indent, i == children.size() - 1);
        }
    }
}

int main() {
    bool showTree = false;

    while (true) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            break;
        }

        if (line == "#showTree") {
            showTree = !showTree;
            std::cout << (showTree ? "Showing parse trees." : "Not showing parse trees") << std::endl;
            continue;
        }


        auto syntaxTree = SyntaxTree::parseToken(line);
        auto binder = new Binder();
        std::vector<std::string_view> diagnostics = syntaxTree->diagnostics();
        // diagnostics.insert(syntaxTree->diagnostics().end(), binder->Diagnostics().begin(), binder->Diagnostics().end());
        auto boundExpression = binder->BindExpression(&syntaxTree->root());

        if (showTree) {
            prettyPrint(syntaxTree->root());
        }

        if (syntaxTree->diagnostics().empty()) {
            Evaluator e(*boundExpression);
            auto result = e.evaluate();
            std::cout << result << std::endl;
        } else {
            for (const auto diagnostic: syntaxTree->diagnostics()) {
                std::cerr << diagnostic << std::endl;
            }
        }
    }
}
