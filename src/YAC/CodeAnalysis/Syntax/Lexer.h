#ifndef LEXER_H
#define LEXER_H
#include <list>
#include <string>
#include <YAC/CodeAnalysis/Text/SourceText.h>
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"


class Lexer {
public:
    // Constructor to initialize the Lexer with input text
    Lexer(const SourceText& text);

    // Destructor
    ~Lexer() = default;

    // get the list of tokens
    std::list<SyntaxToken> tokenize();

private:
    SourceText _text;   // The input text to tokenize
    int _position;                // Current position in inputText
    DiagnosticBag  *diagnosticBag;  //Creating a diagnosticBag

    // Helper function to increase the position
    void advance();

    // Helper function to get the current character
    char getCurrentChar() const;

    char peek(int offset) const;

    // Helper function to get the next token from input text
    SyntaxToken nextToken();

public:
    [[nodiscard]] DiagnosticBag * diagnostic_bag() const {
        return diagnosticBag;
    }
};

#endif // LEXER_H
