#ifndef LEXER_H
#define LEXER_H
#include <list>
#include <string>
#include "Compiler/Syntax.h"

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

    // Helper function to increase the position
    void advance();

    // Helper function to get the current character
    char getCurrentChar() const;

    // Helper function to get the next token from input text
    SyntaxToken nextToken();
};

#endif // LEXER_H
