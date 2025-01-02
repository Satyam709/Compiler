cmake_minimum_required(VERSION 3.29)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)

# Include directories for headers
include_directories(include)

# Add source files
set(SOURCES
        src/Syntax.cpp
        src/main.cpp
        src/Lexer.cpp
        src/Parser.cpp
        src/Expression.cpp
        src/Evaluator.cpp
        src/SyntaxTree.cpp
        src/Binder.cpp
        src/Program.cpp

        # Header files (optional, for clarity; not strictly required in SOURCES)
        include/Compiler/Lexer.h
        include/Compiler/Syntax.h
        include/Compiler/Parser.h
        include/Compiler/Evaluator.h
        include/Compiler/Expression.h
        include/Compiler/SyntaxTree.h
        include/Compiler/Binder.h  # Add Binder.h here
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
