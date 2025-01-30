#include "CodeAnalysis/Syntax/Lexer.h"
#include <cctype>
#include <iostream>

#include "CodeAnalysis/Syntax/SyntaxFacts.h"

Lexer::Lexer(const std::string inputText)
    : _inputText(inputText), _position(0), _len(inputText.size()) {
}

std::list<SyntaxToken> Lexer::tokenize() {
    std::list<SyntaxToken> tokens;
    SyntaxToken token;
    while ((token = nextToken()).kind != SyntaxKind::EndOfFileToken) {
        if (token.kind != SyntaxKind::WhitespaceToken && token.kind != SyntaxKind::BadToken)
            tokens.push_back(token);
    }
    tokens.push_back(token); // Add the EOF token
    return tokens;
}


void Lexer::advance() {
    ++_position;
}



char Lexer::getCurrentChar() const {
    return peek(0);
}

char Lexer::peek(const int offset) const {
    return _position+offset < _len ? _inputText[_position+offset] : '\0';
}

SyntaxToken Lexer::nextToken() {
    if (_position >= _len) {
        return {_position, SyntaxKind::EndOfFileToken, "EOF", nullptr};
    }

    const char current = getCurrentChar();

    if (std::isdigit(current)) {
        int start = _position;
        while (std::isdigit(getCurrentChar())) {
            advance();
        }
        std::string tokenText = _inputText.substr(start, _position - start);
        return {start,SyntaxKind:: NumberToken, tokenText, std::stoi(std::string(tokenText))};
    }
    if (std::isspace(current)) {
        int start = _position;
        while (std::isspace(getCurrentChar())) {
            advance();
        }
        std::string tokenText = _inputText.substr(start, _position - start);
        return {start, SyntaxKind::WhitespaceToken, tokenText, nullptr};
    }

    if(std::isalpha(current)) {
        const int start = _position;
        while (std::isalpha(getCurrentChar())) {
            advance();
        }

        std::string tokenText = _inputText.substr(start, _position - start);
        auto kind = SyntaxFacts::getKeywordKind(tokenText);
        return {start, kind, tokenText, nullptr};
    }

    if (current == '+') {
        return {_position++, SyntaxKind::PlusToken, "+", nullptr};
    }
    if (current == '-') {
        return {_position++, SyntaxKind::MinusToken, "-", nullptr};
    }
    if (current == '*') {
        return {_position++, SyntaxKind::StarToken, "*", nullptr};
    }
    if (current == '/') {
        return {_position++, SyntaxKind::SlashToken, "/", nullptr};
    }
    if (current == '(') {
        return {_position++, SyntaxKind::OpenParenthesisToken, "(", nullptr};
    }
    if (current == ')') {
        return {_position++, SyntaxKind::CloseParenthesisToken, ")", nullptr};
    }
    if (current == '!') {
        if (peek(1)=='=') {
            int start = _position;
            _position+=2;
            return {start, SyntaxKind::NotEqualToken, "!=", nullptr};
        }
        return {_position++, SyntaxKind::BangToken, "!", nullptr};
    }
    if (current == '&' && peek(1)== '&') {
        int start = _position;
        _position+=2;
        return {start, SyntaxKind::AmpersandAmpersandToken, "&&", nullptr};
    }
    if (current == '|' && peek(1)== '|') {
        int start = _position;
        _position+=2;
        return {start, SyntaxKind::PipePipeToken, "||", nullptr};
    }
    if (current == '=' && peek(1)== '=') {
        int start = _position;
        _position+=2;
        return {start, SyntaxKind::EqualEqualToken, "==", nullptr};

    }

    return {_position++, SyntaxKind::BadToken, std::string(&current, 1), nullptr};
}
