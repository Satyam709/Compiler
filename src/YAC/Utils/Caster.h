//
// Created by satya on 23-01-2025.
//

#ifndef CASTER_H
#define CASTER_H
#include <any>
#include <iostream>
#include <ostream>
#include <string>

inline void printAnyValue(const std::any& val) {
    if (val.type() == typeid(int)) {
        std::cout << "int: " << std::any_cast<int>(val) << std::endl;
    } else if (val.type() == typeid(bool)) {
        std::cout << "bool: " << (std::any_cast<bool>(val) ? "true" : "false") << std::endl;
    } else if (val.type() == typeid(double)) {
        std::cout << "double: " << std::any_cast<double>(val) << std::endl;
    } else if (val.type() == typeid(float)) {
        std::cout << "float: " << std::any_cast<float>(val) << std::endl;
    } else if (val.type() == typeid(char)) {
        std::cout << "char: " << std::any_cast<char>(val) << std::endl;
    } else if (val.type() == typeid(std::string)) {
        std::cout << "string: " << std::any_cast<std::string>(val) << std::endl;
    } else {
        std::cout << "Unknown type with type ID: " << val.type().name() << std::endl;
    }
}

inline void printAnyType(const std::type_info& type) {
    if (type == typeid(int)) {
        std::cout << "int" << std::endl;
    } else if (type == typeid(bool)) {
        std::cout << "bool" << std::endl;
    } else if (type == typeid(double)) {
        std::cout << "double" << std::endl;
    } else if (type == typeid(float)) {
        std::cout << "float" << std::endl;
    } else if (type == typeid(char)) {
        std::cout << "char" << std::endl;
    } else if (type == typeid(std::string)) {
        std::cout << "string" << std::endl;
    } else {
        std::cout << "Unknown type: " << type.name() << std::endl;
    }
}


inline std::string getTypeName(const std::type_info& type) {
    if (type == typeid(int)) {
        return "int";
    } else if (type == typeid(bool)) {
        return "bool";
    } else if (type == typeid(double)) {
        return "double";
    } else if (type == typeid(float)) {
        return "float";
    } else if (type == typeid(char)) {
        return "char";
    } else if (type == typeid(std::string)) {
        return "string";
    } else {
        return "unknown";
    }
}

#endif //CASTER_H
