#include "TextSpan.h"

TextSpan::TextSpan(int start, int length)
    : m_start(start), m_length(length) {}

int32_t TextSpan::Start() const {
    return m_start;
}

int32_t TextSpan::Length() const {
    return m_length;
}

int32_t TextSpan::End() const {
    return m_start + m_length;
}
