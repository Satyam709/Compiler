//
// Created by satya on 28-02-2025.
//

#ifndef ASSERTINGENUMERATOR_H
#define ASSERTINGENUMERATOR_H
#include <memory>
#include <vector>


enum class SyntaxKind;
class SyntaxNode;

class AssertingEnumerator {
public:
    explicit AssertingEnumerator(SyntaxNode* node);
    ~AssertingEnumerator();

    void AssertNode(SyntaxKind kind);
    void AssertToken(SyntaxKind kind, const std::string& text);

private:
    bool MarkFailed();
    static std::vector<SyntaxNode*> Flatten(SyntaxNode* node);

    std::vector<SyntaxNode*> _nodes;
    size_t _position;
    bool _hasErrors;
};



#endif //ASSERTINGENUMERATOR_H
