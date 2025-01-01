//
// Created by satya on 01-01-2025.
//

#include "../include/Compiler/SyntaxFacts.h"

int SyntaxFacts::getPrecedence(const SyntaxKind &kind) {
    switch (kind) {
        case PlusToken:
        case MinusToken:
            return 1;
        case StarToken:
        case SlashToken:
            return 2;
        default: return 0;
    }
}
