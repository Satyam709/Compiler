cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)

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
        src/Program.cpp
        src/YAC/CodeAnalysis/Syntax/SyntaxFacts.cpp
        src/YAC/CodeAnalysis/Syntax/SyntaxFacts.h
        src/YAC/CodeAnalysis/Binding/Binder.h
        src/YAC/Utils/Caster.h
        src/YAC/CodeAnalysis/Binding/BoundOperators.h
        src/YAC/CodeAnalysis/Binding/BoundOperators.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
