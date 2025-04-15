//
// Created by satya on 15-04-2025.
//

#include "BoundScope.h"

BoundScope::BoundScope(BoundScope &parent) :_parent(_parent){
}

bool BoundScope::tryDeclare(VariableSymbol &variable) {
    if (_variables.contains(variable.getName())) {
        return false;
    }

    _variables.insert({variable.getName(),variable});
    return true;
}

bool BoundScope::tryLookup(std::string &name, VariableSymbol &variable) {
    const auto it = _variables.find(name);
    if (it != _variables.end()) {
        variable =  it->second;
        return true;
    }

    if (&_parent == nullptr)
        return false;

    return _parent.tryLookup(name,variable);
}




