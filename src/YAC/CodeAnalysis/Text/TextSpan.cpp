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

TextSpan TextSpan::FromBounds(int32_t start, int32_t end) {
    int32_t length = end - start;
    return TextSpan(start, length);
}

bool TextSpan::Contains(int32_t position) const {
    return position >= m_start && position < m_start + m_length;
}

bool TextSpan::OverlapsWith(const TextSpan& span) const {
    return m_start < span.m_start + span.m_length &&
           span.m_start < m_start + m_length;
}