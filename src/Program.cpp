#include <iostream>
#include <string>
#include <vector>

#include "Compiler/Evaluator.h"
#include "Compiler/SyntaxTree.h"


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

        if (showTree) {
            SyntaxTree::prettyPrint(syntaxTree->root());
        }

        if (syntaxTree->diagnostics().empty()) {
            Evaluator e(syntaxTree->root());
            const auto result = e.evaluate();
            std::cout << result << std::endl;
        } else {
            for (const auto &diagnostic: syntaxTree->diagnostics()) {
                std::cerr << diagnostic << std::endl;
            }
        }
    }
}
