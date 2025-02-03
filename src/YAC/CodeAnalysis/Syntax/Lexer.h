#ifndef LEXER_H
#define LEXER_H
#include <list>
#include <string>
#include "Syntax.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"

class Lexer {
public:
    // Constructor to initialize the Lexer with input text
    Lexer(const std::string inputText);

    // Destructor
    ~Lexer() = default;

    // get the list of tokens
    std::list<SyntaxToken> tokenize();

private:
    std::string _inputText;  // The input text to tokenize
    const unsigned _len;          // Length of input text
    int _position;                // Current position in inputText
    DiagnosticBag  *diagnosticBag;  //Creating a diagnosticBag

    // Helper function to increase the position
    void advance();

    // Helper function to get the current character
    char getCurrentChar() const;

    char peek(int offset) const;

    // Helper function to get the next token from input text
    SyntaxToken nextToken();


};

#endif // LEXER_H
