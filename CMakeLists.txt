cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${CMAKE_SOURCE_DIR}/src)

# Add source files
set(SOURCES
        src/YAC/CodeAnalysis/Syntax/Syntax.cpp
        src/YAC/CodeAnalysis/Syntax/Lexer.h
        src/YAC/CodeAnalysis/Syntax/Syntax.h
        src/YAC/CodeAnalysis/Syntax/Lexer.cpp
        src/YAC/CodeAnalysis/Syntax/Parser.cpp
        src/YAC/CodeAnalysis/Syntax/Parser.h
        src/YAC/CodeAnalysis/Syntax/Expression.cpp
        src/YAC/CodeAnalysis/Evaluator.cpp
        src/YAC/CodeAnalysis/Evaluator.h
        src/YAC/CodeAnalysis/Syntax/Expression.h
        src/YAC/CodeAnalysis/Syntax/SyntaxTree.cpp
        src/YAC/CodeAnalysis/Syntax/SyntaxTree.h
        src/YAC/CodeAnalysis/Syntax/CompilationUnitSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/CompilationUnitSyntax.h
        src/Program.cpp
        src/YAC/CodeAnalysis/Syntax/SyntaxFacts.cpp
        src/YAC/CodeAnalysis/Syntax/SyntaxFacts.h
        src/YAC/CodeAnalysis/Binding/Binder.h
        src/YAC/CodeAnalysis/Binding/Binder.cpp
        src/YAC/CodeAnalysis/Binding/BoundNode.h
        src/YAC/CodeAnalysis/Binding/BoundNode.cpp
        src/YAC/Utils/Caster.h
        src/YAC/CodeAnalysis/Binding/BoundOperators.h
        src/YAC/CodeAnalysis/Binding/BoundOperators.cpp
        src/YAC/CodeAnalysis/Compilation.h
        src/YAC/CodeAnalysis/Compilation.cpp
        src/YAC/CodeAnalysis/EvaluationResult.h
        src/YAC/CodeAnalysis/EvaluationResult.cpp
        src/YAC/CodeAnalysis/Text/TextSpan.cpp
        src/YAC/CodeAnalysis/Text/TextSpan.h
        src/YAC/CodeAnalysis/Text/SourceText.cpp
        src/YAC/CodeAnalysis/Text/SourceText.h
        src/YAC/CodeAnalysis/Text/TextLine.cpp
        src/YAC/CodeAnalysis/Text/TextLine.h
        src/YAC/CodeAnalysis/Diagnostics.h
        src/YAC/CodeAnalysis/Diagnostics.cpp
        src/YAC/CodeAnalysis/DiagnosticsBag.h
        src/YAC/CodeAnalysis/DiagnosticsBag.cpp
        src/YAC/CodeAnalysis/Symbols/VariableSymbol.cpp
        src/YAC/CodeAnalysis/Symbols/VariableSymbol.h
        src/YAC/CodeAnalysis/Binding/BoundScope.cpp
        src/YAC/CodeAnalysis/Binding/BoundScope.h
        src/YAC/CodeAnalysis/Binding/BoundGlobalScope.cpp
        src/YAC/CodeAnalysis/Binding/BoundGlobalScope.h
        src/YAC/CodeAnalysis/Syntax/ExpressionStatementSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/ExpressionStatementSyntax.h
        src/YAC/CodeAnalysis/Syntax/BlockStatementSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/BlockStatementSyntax.h
        src/YAC/CodeAnalysis/Binding/BoundExpressionStatement.cpp
        src/YAC/CodeAnalysis/Binding/BoundExpressionStatement.h
        src/YAC/CodeAnalysis/Binding/BoundBlockStatement.cpp
        src/YAC/CodeAnalysis/Binding/BoundBlockStatement.h
        src/YAC/CodeAnalysis/Binding/BoundStatement.h
        src/YAC/CodeAnalysis/Syntax/StatementSyntax.h
        src/YAC/CodeAnalysis/Syntax/VariableDeclarationSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/VariableDeclarationSyntax.h
        src/YAC/CodeAnalysis/Binding/BoundVariableDeclaration.cpp
        src/YAC/CodeAnalysis/Binding/BoundVariableDeclaration.h
        src/YAC/CodeAnalysis/Syntax/ElseClauseSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/ElseClauseSyntax.h
        src/YAC/CodeAnalysis/Syntax/IfStatementSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/IfStatementSyntax.h
        src/YAC/CodeAnalysis/Binding/BoundIfStatement.cpp
        src/YAC/CodeAnalysis/Binding/BoundIfStatement.h
        src/YAC/CodeAnalysis/Syntax/WhileStatementSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/WhileStatementSyntax.h
        src/YAC/CodeAnalysis/Binding/BoundWhileStatement.cpp
        src/YAC/CodeAnalysis/Binding/BoundWhileStatement.h
        src/YAC/CodeAnalysis/Syntax/ForStatementSyntax.cpp
        src/YAC/CodeAnalysis/Syntax/ForStatementSyntax.h
        src/YAC/CodeAnalysis/Binding/BoundForStatement.cpp
        src/YAC/CodeAnalysis/Binding/BoundForStatement.h
)

# Create library from sources
add_library(CompilerLib STATIC ${SOURCES}
        src/YAC/CodeAnalysis/Binding/BoundNode.cpp
        src/YAC/CodeAnalysis/Binding/BoundNode.h
)

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add test directory (Do not define `my_tests` here)
add_subdirectory(src/Tests)
