#pragma once
#include <string>
#include <tuple>

class User {
public:
    int id;
    std::string name;
    double balance;
    bool is_active;

    User() = default;
    User(int id, std::string name, double balance, bool is_active)
        : id(id), name(std::move(name)), balance(balance), is_active(is_active) {}

    static std::string getTableName() { return "users"; }

    static auto getMappings() {
        return std::make_tuple(
            std::make_pair(&User::id, "id"),
            std::make_pair(&User::name, "name"),
            std::make_pair(&User::balance, "balance"),
            std::make_pair(&User::is_active, "is_active")
        );
    }
}; 