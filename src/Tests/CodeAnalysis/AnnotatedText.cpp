//
// Created by satya on 17-04-2025.
//

#include "AnnotatedText.h"
#include <sstream>
#include <stack>
#include <algorithm>
#include <limits>
#include <utility>

AnnotatedText::AnnotatedText(std::string  text, const std::vector<TextSpan>& spans)
    : _text(std::move(text)), _spans(spans) {
}

AnnotatedText AnnotatedText::Parse(const std::string& text) {
    auto unindentedText = Unindent(text);

    std::string resultText;
    std::vector<TextSpan> spans;
    std::stack<int> startStack;

    int position = 0;

    for (char c : unindentedText) {
        if (c == '[') {
            startStack.push(position);
        }
        else if (c == ']') {
            if (startStack.empty()) {
                throw std::invalid_argument("Too many ']' in text");
            }

            int start = startStack.top();
            startStack.pop();
            int end = position;
            spans.push_back(TextSpan::FromBounds(start, end));
        }
        else {
            position++;
            resultText += c;
        }
    }

    if (!startStack.empty()) {
        throw std::invalid_argument("Missing ']' in text");
    }

    return AnnotatedText(resultText, spans);
}

std::string AnnotatedText::Unindent(const std::string& text) {
    auto lines = UnindentLines(text);
    std::string result;

    for (size_t i = 0; i < lines.size(); ++i) {
        result += lines[i];
        if (i < lines.size() - 1) {
            result += "\n";
        }
    }

    return result;
}

std::vector<std::string> AnnotatedText::UnindentLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;

    // Read all lines
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    // Find minimum indentation
    int minIndentation = std::numeric_limits<int>::max();
    for (auto & i : lines) {
        auto& line = i;

        if (line.find_first_not_of(" \t") == std::string::npos) {
            i = "";
            continue;
        }

        int indentation = line.length() - line.find_first_not_of(" \t");
        minIndentation = std::min(minIndentation, indentation);
    }

    // Remove indentation from all lines
    for (auto& line : lines) {
        if (line.empty()) continue;
        line = line.substr(minIndentation);
    }

    // Remove empty lines from start
    while (!lines.empty() && lines.front().empty()) {
        lines.erase(lines.begin());
    }

    // Remove empty lines from end
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }

    return lines;
}