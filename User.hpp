#pragma once
#include <string>
#include <tuple>

class User {
public:
    int id;
    std::string name;
    double balance;

    User() = default;
    User(int id, std::string name, double balance)
        : id(id), name(std::move(name)), balance(balance) {}

    static std::string getTableName() { return "users"; }

    static auto getMappings() {
        return std::make_tuple(
            std::make_pair(&User::id, "id"),
            std::make_pair(&User::name, "name"),
            std::make_pair(&User::balance, "balance")
        );
    }
}; 