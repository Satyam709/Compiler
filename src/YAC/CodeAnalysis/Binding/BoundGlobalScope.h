//
// Created by satya on 15-04-2025.
//

#ifndef BOUNDGLOBALSCOPE_H
#define BOUNDGLOBALSCOPE_H
#include <vector>

#include "YAC/CodeAnalysis/Diagnostics.h"
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"


class BoundStatement;

class BoundGlobalScope final {
public:
    BoundGlobalScope(BoundGlobalScope* previous, std::vector<Diagnostic> diagnostics,
                     std::vector<VariableSymbol> variables,const BoundStatement *expression);

    [[nodiscard]] BoundGlobalScope* previous() const {
        return _previous;
    }

    [[nodiscard]] std::vector<Diagnostic> diagnostics() const {
        return _diagnostics;
    }

    [[nodiscard]] std::vector<VariableSymbol> variables() const {
        return _variables;
    }

    [[nodiscard]]const BoundStatement *statement() const {
        return _statement;
    }


private:
    BoundGlobalScope *_previous;
    std::vector<Diagnostic> _diagnostics;
    std::vector<VariableSymbol> _variables;
    const BoundStatement *_statement;
};


#endif //BOUNDGLOBALSCOPE_H
