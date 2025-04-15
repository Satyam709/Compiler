#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "YAC/CodeAnalysis/Evaluator.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"
#include "YAC/Utils/Caster.h"
#include "YAC/CodeAnalysis/Compilation.h"
#include "YAC/CodeAnalysis/Syntax/CompilationUnitSyntax.h"

// For Windows color support
#ifdef _WIN32
#include <windows.h>
#endif

// Helper function to set console color
void setConsoleColor(bool isError = false) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (isError)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    else
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    if (isError)
        std::cout << "\033[31m";  // Dark red
    else
        std::cout << "\033[0m";   // Reset
#endif
}

int main() {
    bool showTree = true;
    std::unordered_map<VariableSymbol, std::any> variables;
    Compilation* previous = nullptr;

    while (true) {
        std::cout << "> ";
        std::string inputText;

        while (true) {
            std::string line;
            std::getline(std::cin, line);
            if (line.empty())break;
            inputText.append(line).append("\r\n");
        }

        if (inputText.empty()) {
            break;
        }

        if (inputText == "#reset") {
            previous = nullptr;
            variables.clear();
        }

        if (inputText == "#showTree") {
            showTree = !showTree;
            std::cout << (showTree ? "Showing parse trees." : "Not showing parse trees") << std::endl;
            continue;
        } else if (inputText == "#cls") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        const auto syntaxTree = SyntaxTree::parse(inputText); // Changed parseToken to parse

        // Compilation and evaluation
        Compilation* compilation = previous == nullptr? new Compilation(*syntaxTree):previous->continuwWith(syntaxTree);
        EvaluationResult result = compilation->evaluate(variables);

        if (showTree) {
            setConsoleColor(false); // Set to dark gray
            SyntaxTree::prettyPrint(syntaxTree->root()->exp());
            setConsoleColor(); // Reset color
        }


        if (!result.diagnostics().empty()) {
            const auto &text = syntaxTree->text();

            for (const auto &diagnostic: result.diagnostics()) {
                std::cout << std::endl;

                const int lineIndex = text.GetLineIndex(diagnostic.getSpan().Start());
                const int lineNumber = lineIndex + 1;
                const int character = diagnostic.getSpan().Start() - text.GetLines()[lineIndex].Start() + 1;

                setConsoleColor(true); // Set to dark red
                std::cout << "(" << lineNumber << ", " << character << "): ";
                std::cout << diagnostic.getMessage() << std::endl;
                setConsoleColor(); // Reset color

                auto prefix = inputText.substr(0, diagnostic.getSpan().Start());
                auto error = inputText.substr(diagnostic.getSpan().Start(), diagnostic.getSpan().Length());
                auto suffix = inputText.substr(diagnostic.getSpan().End());

                std::cout << "    " << prefix;
                setConsoleColor(true); // Set to dark red
                std::cout << error;
                setConsoleColor(); // Reset color
                std::cout << suffix << std::endl;
            }
            std::cout << std::endl;
        } else {
            try {
                printAnyValue(result.value());
                std::cout << std::endl;
                previous = compilation;
            } catch (const std::bad_any_cast &e) {
                std::cerr << "Cannot cast final result to int !!" << e.what() << std::endl;
            }
        }
    }
    return 0;
}
