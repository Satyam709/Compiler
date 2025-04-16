//
// Created by satya on 15-04-2025.
//

#ifndef BOUNDSCOPE_H
#define BOUNDSCOPE_H
#include <string>
#include <unordered_map>
#include "YAC/CodeAnalysis/Symbols/VariableSymbol.h"

class BoundScope final {
public:
    explicit BoundScope(BoundScope *parent);

    bool tryDeclare(VariableSymbol &variable);

    bool tryLookup(std::string &name, VariableSymbol &variable);

    [[nodiscard]] BoundScope *parent() const {
        return _parent;
    }

    [[nodiscard]] std::unordered_map<std::string, VariableSymbol> variables() const {
        return _variables;
    }

    std::vector<VariableSymbol> getDeclaredVariables() const {
        std::vector<VariableSymbol> result = {};
        for (const auto &[fst, snd]: _variables) {
            result.push_back(snd);
        }
        return result;
    }

private:
    BoundScope *_parent;
    std::unordered_map<std::string, VariableSymbol> _variables;
};


#endif //BOUNDSCOPE_H
