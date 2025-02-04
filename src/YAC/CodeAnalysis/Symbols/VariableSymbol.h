#ifndef VARIABLESYMBOL_H
#define VARIABLESYMBOL_H

#include <string>
#include <typeinfo>
#include <functional>

class VariableSymbol {
public:
    VariableSymbol(std::string name, const std::type_info& type);

    const std::string& getName() const;
    const std::type_info& getType() const;

    // Add equality operator
    bool operator==(const VariableSymbol& other) const {
        return _name == other._name;  // Compare only names for now
    }

    bool operator!=(const VariableSymbol& other) const {
        return !(*this == other);
    }

private:
    std::string _name;
    const std::type_info& _type;
};

// Add hash function specialization
namespace std {
    template<>
    struct hash<VariableSymbol> {
        size_t operator()(const VariableSymbol& symbol) const noexcept {
            return hash<string>()(symbol.getName());  // Hash only the name
        }
    };
}

#endif // VARIABLESYMBOL_H