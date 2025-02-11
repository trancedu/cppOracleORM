#pragma once
#include "Database.hpp"
#include <tuple>
#include <variant>
#include <sstream>

using VariantType = std::variant<int, double, std::string, bool>;

template <typename T>
std::string getColumnNames() {
    auto mappings = T::getMappings();
    std::stringstream ss;
    
    for (const auto& pair : mappings) {
        ss << pair.second << ", ";
    }
    
    std::string result = ss.str();
    if (!result.empty()) {
        result.resize(result.size() - 2); // Remove trailing ", "
    }
    return result;
}

template <typename T>
std::vector<VariantType> getValues(const T& obj) {
    auto mappings = T::getMappings();
    std::vector<VariantType> values;

    for (const auto& pair : mappings) {
        std::visit([&](auto&& member) {
            values.push_back(obj.*member);
        }, pair.first);
    }

    return values;
}

template <typename T>
void insertObject(Connection* conn, const T& obj) {
    std::string sql = "INSERT INTO " + T::getTableName() + " (" + getColumnNames<T>() + ") VALUES (";
    auto values = getValues(obj);

    for (size_t i = 0; i < values.size(); i++) sql += "?,";
    sql.pop_back();
    sql += ")";

    Statement* stmt = Database::createStatement(sql);
    
    int index = 1;
    for (const auto& val : values) {
        std::visit([&](auto&& arg) {
            using V = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<V, int>) {
                stmt->setInt(index, arg);
            } else if constexpr (std::is_same_v<V, double>) {
                stmt->setDouble(index, arg);
            } else if constexpr (std::is_same_v<V, std::string>) {
                stmt->setString(index, arg);
            } else if constexpr (std::is_same_v<V, bool>) {
                stmt->setBool(index, arg);
            }
        }, val);
        index++;
    }

    stmt->executeUpdate();
    Database::terminateStatement(stmt);
}

template <typename T>
T getObjectById(Connection* conn, int id) {
    std::string sql = "SELECT " + getColumnNames<T>() + " FROM " + T::getTableName() + " WHERE id = ?";
    Statement* stmt = Database::createStatement(sql);
    stmt->setInt(1, id);
    
    auto rs = stmt->executeQuery();
    T obj;
    
    if (rs->next()) {
        int index = 1;
        auto mappings = T::getMappings();

        for (const auto& pair : mappings) {
            std::visit([&](auto&& member) {
                using ValueType = std::decay_t<decltype(obj.*member)>;
                if constexpr (std::is_same_v<ValueType, int>) {
                    obj.*member = rs->getInt(index);
                } else if constexpr (std::is_same_v<ValueType, double>) {
                    obj.*member = rs->getDouble(index);
                } else if constexpr (std::is_same_v<ValueType, std::string>) {
                    obj.*member = rs->getString(index);
                } else if constexpr (std::is_same_v<ValueType, bool>) {
                    obj.*member = rs->getBool(index);
                }
                index++;
            }, pair.first);
        }
    }

    Database::terminateStatement(stmt);
    return obj;
} 