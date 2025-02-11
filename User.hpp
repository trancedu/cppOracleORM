#pragma once
#include <string>
#include <vector>
#include <variant>

class User {
public:
    int id;
    std::string name;
    double balance;
    bool is_active;

    // Define a variant type for member pointers
    using MemberVariant = std::variant<
        int User::*,
        std::string User::*,
        double User::*,
        bool User::*
    >;

    User() = default;
    User(int id, std::string name, double balance, bool is_active)
        : id(id), name(std::move(name)), balance(balance), is_active(is_active) {}

    static std::string getTableName() { return "users"; }

    static std::vector<std::pair<MemberVariant, std::string>> getMappings() {
        return {
            {&User::id, "id"},
            {&User::name, "name"},
            {&User::balance, "balance"},
            {&User::is_active, "is_active"}
        };
    }
}; 