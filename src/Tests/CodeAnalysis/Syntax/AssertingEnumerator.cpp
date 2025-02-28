//
// Created by satya on 28-02-2025.
//

#include "AssertingEnumerator.h"

#include <ranges>
#include <stack>
#include <gtest/gtest.h>
#include "YAC/CodeAnalysis/Syntax/Syntax.h"
#include "YAC/CodeAnalysis/Syntax/SyntaxTree.h"


AssertingEnumerator::AssertingEnumerator(SyntaxNode* node)
    : _nodes(Flatten(node)), _position(0), _hasErrors(false) {
}

AssertingEnumerator::~AssertingEnumerator() {
    if (!_hasErrors) {
        EXPECT_EQ(_position, _nodes.size()) << "Not all nodes were consumed";
    }
}

bool AssertingEnumerator::MarkFailed() {
    _hasErrors = true;
    return false;
}

std::vector<SyntaxNode*> AssertingEnumerator::Flatten(SyntaxNode* node) {
    std::vector<SyntaxNode*> result;
    std::stack<SyntaxNode*> stack;

    stack.push(node);

    while (!stack.empty()) {
        auto n = stack.top();
        stack.pop();

        result.push_back(n);

        // Get the children and push them onto the stack in reverse order
        auto children = n->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            stack.push(*it);
        }
    }

    return result;
}

void AssertingEnumerator::AssertNode(const SyntaxKind kind) {
    try {
        ASSERT_LT(_position, _nodes.size()) << "Expected more nodes";

        const auto current = _nodes[_position++];
        ASSERT_EQ(current->getKind(), kind) << "Expected node kind " << kind
                                        << " but got " <<current->getKind();

        // Check that it's not a SyntaxNodeToken
        ASSERT_FALSE(dynamic_cast<SyntaxNodeToken*>(current))
            << "Expected node but got token";
    }
    catch (...) {
        MarkFailed();
        throw;
    }
}

void AssertingEnumerator::AssertToken(const SyntaxKind kind, const std::string& text) {
    try {
        ASSERT_LT(_position, _nodes.size()) << "Expected more nodes";

        const auto current = _nodes[_position++];
        ASSERT_EQ(current->getKind(), kind) << "Expected token kind " << kind
                                        << " but got " << current->getKind();

        const auto nodeToken = dynamic_cast<SyntaxNodeToken*>(current);
        ASSERT_TRUE(nodeToken) << "Expected token but got node";

        const auto token = nodeToken->getToken();

        ASSERT_EQ(token.text, text) << "Expected token text '" << text
                                      << "' but got '" << token.text << "'";
    }
    catch (...) {
        MarkFailed();
        throw;
    }
}