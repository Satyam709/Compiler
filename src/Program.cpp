#include <iostream>
#include <string>
#include <vector>

#include "YAC/CodeAnalysis/Evaluator.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"
#include "YAC/Utils/Caster.h"
#include "YAC/CodeAnalysis/Compilation.h"

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

        // Use the Compilation class to handle binding and evaluation
        Compilation compilation(*syntaxTree);
        EvaluationResult result = compilation.evaluate();

        // Check for diagnostics
        if (!result.diagnostics().empty()) {
            for (const auto& diagnostic : result.diagnostics()) {
                std::cerr << diagnostic << std::endl;
            }
        } else {
            // Output the evaluation result
            try {
                printAnyValue(result.value());
            } catch (const std::bad_any_cast& e) {
                std::cerr << "Cannot cast final result to int !!" << std::endl;
            }
        }
    }
}
