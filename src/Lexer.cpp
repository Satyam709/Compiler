#include "Compiler/Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string_view inputText)
    : _inputText(inputText), _position(0), _len(inputText.size()) {
}

std::list<SyntaxToken> Lexer::tokenize() {
    std::list<SyntaxToken> tokens;
    SyntaxToken token;
    while ((token = nextToken()).kind != EndOfFileToken) {
        if (token.kind != WhitespaceToken && token.kind != BadToken)
            tokens.push_back(token);
    }
    tokens.push_back(token); // Add the EOF token
    return tokens;
}


void Lexer::advance() {
    ++_position;
}

char Lexer::getCurrentChar() const {
    return _position < _len ? _inputText[_position] : '\0';
}

SyntaxToken Lexer::nextToken() {
    if (_position >= _len) {
        return {_position, EndOfFileToken, "EOF", nullptr};
    }

    char current = getCurrentChar();

    if (std::isdigit(current)) {
        int start = _position;
        while (std::isdigit(getCurrentChar())) {
            advance();
        }
        std::string_view tokenText = _inputText.substr(start, _position - start);
        return {start, NumberToken, tokenText, std::stoi(std::string(tokenText))};
    }
    if (std::isspace(current)) {
        int start = _position;
        while (std::isspace(getCurrentChar())) {
            advance();
        }
        std::string_view tokenText = _inputText.substr(start, _position - start);
        return {start, WhitespaceToken, tokenText, nullptr};
    }
    if (current == '+') {
        return {_position++, PlusToken, "+", nullptr};
    }
    if (current == '-') {
        return {_position++, MinusToken, "-", nullptr};
    }
    if (current == '*') {
        return {_position++, StarToken, "*", nullptr};
    }
    if (current == '/') {
        return {_position++, SlashToken, "/", nullptr};
    }
    if (current == '(') {
        return {_position++, OpenParenthesisToken, "(", nullptr};
    }
    if (current == ')') {
        return {_position++, CloseParenthesisToken, ")", nullptr};
    }

    return {_position++, BadToken, std::string_view(&current, 1), nullptr};
}

