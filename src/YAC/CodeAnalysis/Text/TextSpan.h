#ifndef TEXTSPAN_H
#define TEXTSPAN_H

#include <cstdint>

class TextSpan {
public:
    TextSpan(int start, int length);

    // Existing methods
    [[nodiscard]] int32_t Start() const;
    [[nodiscard]] int32_t Length() const;
    [[nodiscard]] int32_t End() const;

    // New static method
    static TextSpan FromBounds(int32_t start, int32_t end);

    // Optional utility methods
    [[nodiscard]] bool Contains(int32_t position) const;
    [[nodiscard]] bool OverlapsWith(const TextSpan& span) const;

private:
    int32_t m_start;
    int32_t m_length;
};

#endif // TEXTSPAN_H