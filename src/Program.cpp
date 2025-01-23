#include <iostream>
#include <string>
#include <vector>

#include "CodeAnalysis/Evaluator.h"
#include "CodeAnalysis/Syntax/SyntaxTree.h"


int main() {
    bool showTree = true;

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


        const auto syntaxTree = SyntaxTree::parseToken(line);

        const auto binder = new Binder();
        const auto boundExpression = binder->bindExpression(syntaxTree->root());

        if (showTree) {
            SyntaxTree::prettyPrint(syntaxTree->root());
        }
        auto all_diag = std::vector<std::string>(syntaxTree->diagnostics());
        all_diag.insert(all_diag.end(), binder->diagnostics().begin(), binder->diagnostics().end());

        if (all_diag.empty()) {
            Evaluator e(*boundExpression);
            const auto result = e.evaluate();
            std::cout << result << std::endl;
        } else {
            for (const auto &diagnostic: all_diag) {
                std::cerr << diagnostic << std::endl;
            }
        }
    }
}
