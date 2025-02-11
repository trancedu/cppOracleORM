#pragma once
#include <string>

class User {
public:
    int id;
    std::string name;
    double balance;
    bool is_active;

    User() = default;
    User(int id, std::string name, double balance, bool is_active)
        : id(id), name(std::move(name)), balance(balance), is_active(is_active) {}
}; 