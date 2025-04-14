#pragma once
#include <string>
#include <vector>
#include "TextLine.h"

class SourceText {
public:
    static SourceText From(const std::string& text);
    SourceText(const std::string& text);

    // Accessors
    char operator[](int index) const;
    int Length() const;
    const std::vector<TextLine>& GetLines() const;
    int GetLineIndex(int position) const;

    // String operations
    std::string ToString() const;
    std::string ToString(int start, int length) const;
    std::string ToString(const TextSpan& span) const;

private:

    static std::vector<TextLine> ParseLines(SourceText* sourceText, const std::string& text);
    static void AddLine(std::vector<TextLine>& result, SourceText* sourceText,
                       int position, int lineStart, int lineBreakWidth);
    static int GetLineBreakWidth(const std::string& text, int position);

    std::string _text;
    std::vector<TextLine> _lines;
};