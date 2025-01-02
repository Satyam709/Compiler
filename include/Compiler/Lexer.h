#ifndef LEXER_H
#define LEXER_H
#include <list>
#include <string_view>
#include "Compiler/Syntax.h"
#include <vector>

class Lexer {
public:
    // Constructor to initialize the Lexer with input text
    Lexer(const std::string_view inputText);

    // Destructor
    ~Lexer() = default;

    // get the list of tokens
    std::list<SyntaxToken> tokenize();

    // get the list of diagnostics
    const std::vector<std::string_view> &diagnostics() const;

private:
    std::string_view _inputText;  // The input text to tokenize
    const unsigned _len;          // Length of input text
    int _position;                // Current position in inputText
    std::vector<std::string_view> _diagnostics; // List of diagnostics/errors

    // Helper function to increase the position
    void advance();

    // Helper function to get the current character
    char getCurrentChar() const;

    // Helper function to get the next token from input text
    SyntaxToken nextToken();
};

#endif // LEXER_H
