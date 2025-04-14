#include "TextLine.h"
#include "SourceText.h"

TextLine::TextLine(SourceText* text, int start, int length, int lengthIncludingLineBreak)
    : _text(text)
    , _start(start)
    , _length(length)
    , _lengthIncludingLineBreak(lengthIncludingLineBreak)
{
}

TextSpan TextLine::Span() const {
    return TextSpan(_start, _length);
}

TextSpan TextLine::SpanIncludingLineBreak() const {
    return TextSpan(_start, _lengthIncludingLineBreak);
}

std::string TextLine::ToString() const {
    return _text->ToString(Span());
}