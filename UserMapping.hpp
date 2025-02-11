#pragma once
#include "User.hpp"
#include "ORM.hpp"

template<>
struct ORM::EntityMapping<User> {
    static constexpr std::string table_name = "users";
    
    static constexpr auto mappings = std::make_tuple(
        std::make_pair(&User::id, "id"),
        std::make_pair(&User::name, "name"),
        std::make_pair(&User::balance, "balance"),
        std::make_pair(&User::is_active, "is_active")
    );
}; 