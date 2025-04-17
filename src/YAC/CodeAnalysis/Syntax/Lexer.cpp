#include "Lexer.h"
#include <cctype>
#include <iostream>
#include "SyntaxFacts.h"
#include "YAC/CodeAnalysis/DiagnosticsBag.h"
#include "YAC/Utils/Caster.h"
#include "YAC/CodeAnalysis/Text/SourceText.h"  // Add this include

// Update constructor to use SourceText
Lexer::Lexer(const SourceText &text)
    : _text(text), _position(0), diagnosticBag(new DiagnosticBag()) {
}

std::list<SyntaxToken> Lexer::tokenize() {
    std::list<SyntaxToken> tokens;
    SyntaxToken token;
    while ((token = nextToken()).kind != SyntaxKind::EndOfFileToken) {
        if (token.kind != SyntaxKind::WhitespaceToken && token.kind != SyntaxKind::BadToken)
            tokens.push_back(token);
    }
    tokens.push_back(token);
    return tokens;
}

void Lexer::advance() {
    ++_position;
}

char Lexer::getCurrentChar() const {
    return peek(0);
}

char Lexer::peek(const int offset) const {
    int index = _position + offset;
    return index < _text.Length() ? _text[index] : '\0';
}

SyntaxToken Lexer::nextToken() {
    if (_position >= _text.Length()) {
        return {_position, SyntaxKind::EndOfFileToken, "", nullptr};
    }

    const char current = getCurrentChar();

    if (std::isdigit(current)) {
        int start = _position;
        while (std::isdigit(getCurrentChar())) {
            advance();
        }
        const std::string tokenText = _text.ToString(start, _position - start);
        SyntaxToken token = {start, SyntaxKind::NumberToken, tokenText, nullptr};

        try {
            token.val = std::stoi(tokenText);
            return token;
        } catch (const std::exception &e) {
            diagnosticBag->reportInvalidNumber(token.getSpan(), token.text, "int");
        }
    }

    if (std::isspace(current)) {
        int start = _position;
        while (std::isspace(getCurrentChar())) {
            advance();
        }
        std::string tokenText = _text.ToString(start, _position - start);
        return {start, SyntaxKind::WhitespaceToken, tokenText, nullptr};
    }

    if (std::isalpha(current)) {
        const int start = _position;
        while (std::isalpha(getCurrentChar())) {
            advance();
        }

        std::string tokenText = _text.ToString(start, _position - start);
        auto kind = SyntaxFacts::getKeywordKind(tokenText);
        return {start, kind, tokenText, nullptr};
    }

    // Single-character tokens remain mostly the same
    if (current == '+') {
        return {_position++, SyntaxKind::PlusToken, "+", nullptr};
    }
    if (current == '-') {
        return {_position++, SyntaxKind::MinusToken, "-", nullptr};
    }
    if (current == '*') {
        return {_position++, SyntaxKind::StarToken, "*", nullptr};
    }
    if (current == '<') {
        if (peek(1) == '=') {
            int start = _position;
            _position += 2;
            return {start, SyntaxKind::LessOrEqualsToken, "<=", nullptr};
        }
        return {_position++, SyntaxKind::LessToken, "<", nullptr};
    }
    if (current == '>') {
        if (peek(1) == '=') {
            int start = _position;
            _position += 2;
            return {start, SyntaxKind::GreaterOrEqualsToken, ">=", nullptr};
        }
        return {_position++, SyntaxKind::GreaterToken, ">", nullptr};
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
    if (current == '{') {
        return {_position++, SyntaxKind::OpenBraceToken, "{", nullptr};
    }
    if (current == '}') {
        return {_position++, SyntaxKind::CloseBraceToken, "}", nullptr};
    }
    if (current == '!') {
        if (peek(1) == '=') {
            int start = _position;
            _position += 2;
            return {start, SyntaxKind::NotEqualToken, "!=", nullptr};
        }
        return {_position++, SyntaxKind::BangToken, "!", nullptr};
    }
    if (current == '&' && peek(1) == '&') {
        int start = _position;
        _position += 2;
        return {start, SyntaxKind::AmpersandAmpersandToken, "&&", nullptr};
    }
    if (current == '|' && peek(1) == '|') {
        int start = _position;
        _position += 2;
        return {start, SyntaxKind::PipePipeToken, "||", nullptr};
    }
    if (current == '=') {
        if (peek(1) == '=') {
            int start = _position;
            _position += 2;
            return {start, SyntaxKind::EqualEqualToken, "==", nullptr};
        } else {
            int start = _position;
            _position += 1;
            return {start, SyntaxKind::EqualsToken, "=", nullptr};
        }
    }

    SyntaxToken bad_token = {_position++, SyntaxKind::BadToken, std::string(&current, 1), nullptr};
    diagnosticBag->reportBadCharacter(bad_token.position, bad_token.text[0]);
    return bad_token;
}
