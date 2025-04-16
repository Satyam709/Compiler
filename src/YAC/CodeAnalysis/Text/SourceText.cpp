#include "SourceText.h"

SourceText::SourceText(const std::string& text)
    : _text(text) {
    _lines = ParseLines(this, text);
}

char SourceText::operator[](int index) const {
    return _text[index];
}

int SourceText::Length() const {
    return _text.length();
}

const std::vector<TextLine>& SourceText::GetLines() const {
    return _lines;
}

int SourceText::GetLineIndex(int position) const{
    int lower = 0;
    int upper = _lines.size() - 1;

    while (lower <= upper) {
        int index = lower + (upper - lower) / 2;
        int start = _lines[index].Start();

        if (position == start)
            return index;

        if (start > position) {
            upper = index - 1;
        } else {
            lower = index + 1;
        }
    }

    return lower - 1;
}

std::vector<TextLine> SourceText::ParseLines(SourceText* sourceText, const std::string& text) {
    std::vector<TextLine> result;

    int position = 0;
    int lineStart = 0;

    while (position <= text.length()) {
        int lineBreakWidth = GetLineBreakWidth(text, position);

        if (lineBreakWidth == 0) {
            position++;
        } else {
            AddLine(result, sourceText, position, lineStart, lineBreakWidth);

            position += lineBreakWidth;
            lineStart = position;
        }
    }

    if (position > lineStart)
        AddLine(result, sourceText, position, lineStart, 0);

    return result;
}

void SourceText::AddLine(std::vector<TextLine>& result, SourceText* sourceText,
                        int position, int lineStart, int lineBreakWidth) {
    int lineLength = position - lineStart;
    int lineLengthIncludingLineBreak = lineLength + lineBreakWidth;
    TextLine line(sourceText, lineStart, lineLength, lineLengthIncludingLineBreak);
    result.push_back(line);
}

int SourceText::GetLineBreakWidth(const std::string& text, int position) {
    char c = text[position];
    char l = position + 1 >= text.length() ? '\0' : text[position + 1];

    if (c == '\r' && l == '\n')
        return 2;

    if (c == '\r' || c == '\n')
        return 1;

    return 0;
}

SourceText SourceText::From(const std::string& text) {
    return SourceText(text);
}

std::string SourceText::ToString() const {
    return _text;
}

std::string SourceText::ToString(int start, int length) const {
    return _text.substr(start, length);
}

std::string SourceText::ToString(const TextSpan& span) const {
    return ToString(span.Start(), span.Length());
}