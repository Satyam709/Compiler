cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)

# Include directories for headers
include_directories(include)

# Add source files
set(SOURCES
        src/CodeAnalysis/Syntax/Syntax.cpp
        include/CodeAnalysis/Syntax/Lexer.h
        include/CodeAnalysis/Syntax/Syntax.h
        src/CodeAnalysis/Syntax/Lexer.cpp
        src/CodeAnalysis/Syntax/Parser.cpp
        include/CodeAnalysis/Syntax/Parser.h
        src/CodeAnalysis/Syntax/Expression.cpp
        src/CodeAnalysis/Evaluator.cpp
        include/CodeAnalysis/Evaluator.h
        include/CodeAnalysis/Syntax/Expression.h
        src/CodeAnalysis/Syntax/SyntaxTree.cpp
        include/CodeAnalysis/Syntax/SyntaxTree.h
        src/Program.cpp
        src/CodeAnalysis/Syntax/SyntaxFacts.cpp
        include/CodeAnalysis/Syntax/SyntaxFacts.h
        include/CodeAnalysis/Binding/Binder.h
        include/CodeAnalysis/Utils/Caster.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
