//
// Created by satya on 17-04-2025.
//

#include "AnnotatedText.h"
#include <sstream>
#include <stack>
#include <algorithm>
#include <limits>
#include <utility>

AnnotatedText::AnnotatedText(std::string text, const std::vector<TextSpan> &spans)
    : _text(std::move(text)), _spans(spans) {
}

AnnotatedText AnnotatedText::Parse(const std::string &text) {
    auto unindentedText = Unindent(text);

    std::string resultText;
    std::vector<TextSpan> spans;
    std::stack<int> startStack;

    int position = 0;

    for (char c: unindentedText) {
        if (c == '[') {
            startStack.push(position);
        } else if (c == ']') {
            if (startStack.empty()) {
                throw std::invalid_argument("Too many ']' in text");
            }

            const int start = startStack.top();
            startStack.pop();
            const int end = position;
            spans.push_back(TextSpan::FromBounds(start, end));
        } else {
            position++;
            resultText += c;
        }
    }

    if (!startStack.empty()) {
        throw std::invalid_argument("Missing ']' in text");
    }

    return AnnotatedText(resultText, spans);
}

std::string AnnotatedText::Unindent(const std::string &text) {
    const auto lines = UnindentLines(text);
    std::string result;

    for (size_t i = 0; i < lines.size(); ++i) {
        result += lines[i];
        if (i < lines.size() - 1) {
            result += "\n";
        }
    }

    return result;
}

std::vector<std::string> AnnotatedText::UnindentLines(const std::string &text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;

    // Read all lines
    while (std::getline(stream, line)) {
        // Skip empty lines at the start and end
        if (line.find_first_not_of(" \t\r\n") != std::string::npos) {
            lines.push_back(line);
        }
    }

    if (lines.empty()) {
        return lines;
    }

    // Find minimum indentation
    size_t minIndentation = std::string::npos;
    for (const auto &line: lines) {
        if (line.empty()) continue;

        size_t indentation = line.find_first_not_of(" \t");
        if (indentation != std::string::npos) {
            minIndentation = (minIndentation == std::string::npos)
                                 ? indentation
                                 : std::min(minIndentation, indentation);
        }
    }

    // Remove indentation from all lines
    if (minIndentation != std::string::npos) {
        for (auto &line: lines) {
            if (!line.empty() && line.length() > minIndentation) {
                line = line.substr(minIndentation);
            }
        }
    }

    // Trim each line
    for (auto &line: lines) {
        // Remove leading whitespace
        size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos) {
            line = line.substr(start);
        }

        // Remove trailing whitespace
        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        }
    }
    return lines;
}
