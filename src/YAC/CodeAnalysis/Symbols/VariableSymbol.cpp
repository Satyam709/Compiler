#include "VariableSymbol.h"

VariableSymbol::VariableSymbol(std::string name, const std::type_info& type)
    : _name(std::move(name)), _type(&type) {
}

const std::string& VariableSymbol::getName() const {
    return _name;
}

const std::type_info& VariableSymbol::getType() const {
    return *_type;
}