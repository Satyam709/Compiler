#include "VariableSymbol.h"

VariableSymbol::VariableSymbol(std::string name, const bool isReadOnly, const std::type_info& type)
    : _name(std::move(name)), _isReadOnly(isReadOnly), _type(&type) {
}

const std::string& VariableSymbol::getName() const {
    return _name;
}

const std::type_info& VariableSymbol::getType() const {
    return *_type;
}

const bool & VariableSymbol::isReadOnly() const {
    return _isReadOnly;
}