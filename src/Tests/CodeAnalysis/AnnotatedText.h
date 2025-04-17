//
// Created by satya on 17-04-2025.
//

#pragma once
#include <string>
#include <vector>
#include "YAC/CodeAnalysis/Text/TextSpan.h"

class AnnotatedText {
public:
    AnnotatedText(std::string  text, const std::vector<TextSpan>& spans);

    [[nodiscard]] const std::string& getText() const { return _text; }
    [[nodiscard]] const std::vector<TextSpan>& getSpans() const { return _spans; }

    static AnnotatedText Parse(const std::string& text);

private:
    static std::string Unindent(const std::string& text);
    static std::vector<std::string> UnindentLines(const std::string& text);

    std::string _text;
    std::vector<TextSpan> _spans;
};
