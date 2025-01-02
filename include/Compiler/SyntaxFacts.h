#ifndef SYNTAXFACTS_H
#define SYNTAXFACTS_H
#include "Syntax.h"

class SyntaxFacts { 
public:
    static int getUnaryOperatorPrecedence(SyntaxKind kind){
        switch (kind) {
            case PlusToken:
            case MinusToken:
                return 3;
            default:
                return 0;
        }
    }
    static int getBinaryOperatorPrecedence(SyntaxKind kind){
        switch (kind) {
            case StarToken:
            case SlashToken:
                return 2;
            case PlusToken:
            case MinusToken:
                return 1;
            default:
                return 0;
        }
    }
};

#endif //SYNTAXFACTS_H