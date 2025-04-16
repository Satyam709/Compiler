//
// Created by satya on 15-04-2025.
//
#include "BoundGlobalScope.h"
#include <utility>

BoundGlobalScope::BoundGlobalScope(BoundGlobalScope *previous, std::vector<Diagnostic> diagnostics,
                                   std::vector<VariableSymbol> variables,
                                   const BoundStatement *expression) : _previous(previous), _diagnostics(std::move(diagnostics)),
                                                                  _variables(std::move(variables)), _statement(expression) {
}
