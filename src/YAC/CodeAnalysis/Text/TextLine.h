#pragma once
#include <string>
#include "TextSpan.h"

class SourceText; // Forward declaration

class TextLine {
public:
    TextLine(SourceText* text, int start, int length, int lengthIncludingLineBreak);

    // Getters
    SourceText* GetText() const { return _text; }
    int Start() const { return _start; }
    int Length() const { return _length; }
    int End() const { return _start + _length; }
    int LengthIncludingLineBreak() const { return _lengthIncludingLineBreak; }

    TextSpan Span() const;
    TextSpan SpanIncludingLineBreak() const;
    std::string ToString() const;

private:
    SourceText* _text;
    int _start;
    int _length;
    int _lengthIncludingLineBreak;
};