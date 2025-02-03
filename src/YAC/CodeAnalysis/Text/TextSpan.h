#ifndef TEXT_SPAN_H
#define TEXT_SPAN_H

#include <cstdint>

class TextSpan {
public:
    TextSpan(int start, int length);

    int Start() const;
    int Length() const;
    int End() const;

private:
    int m_start;
    int m_length;
};

#endif // TEXT_SPAN_H
