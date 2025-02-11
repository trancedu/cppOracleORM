#pragma once
#include "Database.hpp"
#include <tuple>
#include <sstream>
#include <concepts>

namespace ORM {
    template<typename T>
    struct EntityMapping;  // Forward declaration

    template<typename T>
    concept ORMEntity = requires {
        typename EntityMapping<T>;  // Ensure specialization exists
        { EntityMapping<T>::table_name } -> std::convertible_to<std::string>;
        requires std::tuple_size_v<decltype(EntityMapping<T>::mappings)> > 0;
    };

    template<ORMEntity T>
    constexpr auto get_member_pointers() {
        return std::apply([](auto&&... pairs) {
            return std::make_tuple(pairs.first...);
        }, EntityMapping<T>::mappings);
    }

    template<ORMEntity T>
    std::string get_column_names() {
        std::stringstream ss;
        std::apply([&ss](auto&&... pairs) {
            ((ss << pairs.second << ", "), ...);
        }, EntityMapping<T>::mappings);
        
        std::string result = ss.str();
        if (!result.empty()) result.resize(result.size() - 2);
        return result;
    }

    template<ORMEntity T>
    auto get_values(const T& obj) {
        return std::apply([&obj](auto&&... pairs) {
            return std::make_tuple((obj.*(pairs.first))...);
        }, EntityMapping<T>::mappings);
    }

    template<ORMEntity T>
    void insertObject(Connection* conn, const T& obj) {
        constexpr size_t param_count = std::tuple_size_v<decltype(EntityMapping<T>::mappings)>;
        std::string sql = "INSERT INTO " + EntityMapping<T>::table_name + " (" + 
                         get_column_names<T>() + ") VALUES (";
        
        // Generate proper placeholders
        for(size_t i = 0; i < param_count; ++i) {
            sql += (i == 0) ? "?" : ",?";
        }
        sql += ")";

        Statement* stmt = Database::createStatement(sql);
        
        std::apply([stmt](const auto&... values) {
            int index = 1;
            ((stmt->set_param(index++, values)), ...);
        }, get_values(obj));

        stmt->executeUpdate();
        Database::terminateStatement(stmt);
    }

    template<ORMEntity T>
    T getObjectById(Connection* conn, int id) {
        std::string sql = "SELECT " + get_column_names<T>() + " FROM " + 
                         EntityMapping<T>::table_name + " WHERE id = ?";
        Statement* stmt = Database::createStatement(sql);
        stmt->setInt(1, id);
        
        auto rs = stmt->executeQuery();
        T obj;
        
        if (rs->next()) {
            std::apply([&obj, rs](auto&&... pairs) {
                int index = 1;
                ((rs->set_value(obj.*(pairs.first), index++)), ...);
            }, EntityMapping<T>::mappings);
        }

        Database::terminateStatement(stmt);
        return obj;
    }
} 