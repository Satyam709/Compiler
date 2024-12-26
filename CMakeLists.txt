cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)

# Include directories for headers
include_directories(include)

# Add source files
set(SOURCES
        src/Syntax.cpp
        src/main.cpp
        include/Compiler/Lexer.h
        include/Compiler/Syntax.h
        src/Lexer.cpp
        src/Parser.cpp
        include/Compiler/Parser.h
        src/Expression.cpp
        src/Evaluator.cpp
        include/Compiler/Evaluator.h
        include/Compiler/Expression.h
        src/SyntaxTree.cpp
        include/Compiler/SyntaxTree.h
        src/Program.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
