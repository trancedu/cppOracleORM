#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <memory>

// Mock Oracle OCCI-like interfaces
struct Connection {
    static Connection* createConnection(const std::string&, const std::string&, const std::string&) {
        return new Connection();
    }
};

struct Statement {
    std::string sql;
    std::vector<std::variant<int, double, std::string, bool>> params;
    
    void setInt(int index, int value) { 
        std::cout << "Setting INT param [" << index << "] = " << value << "\n";
        params.emplace_back(value); 
    }
    
    void setDouble(int index, double value) { 
        std::cout << "Setting DOUBLE param [" << index << "] = " << value << "\n";
        params.emplace_back(value); 
    }
    
    void setString(int index, const std::string& value) { 
        std::cout << "Setting STRING param [" << index << "] = " << value << "\n";
        params.emplace_back(value); 
    }
    
    void setBool(int index, bool value) { 
        std::cout << "Setting BOOL param [" << index << "] = " << std::boolalpha << value << "\n";
        params.emplace_back(value);
    }
    
    template<typename T>
    void set_param(int index, const T& value) {
        if constexpr (std::is_same_v<T, int>) {
            setInt(index, value);
        } else if constexpr (std::is_same_v<T, double>) {
            setDouble(index, value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            setString(index, value);
        } else if constexpr (std::is_same_v<T, bool>) {
            setBool(index, value);
        }
    }
    
    struct ResultSet {
        int currentRow = -1;
        std::vector<std::vector<std::variant<int, double, std::string, bool>>> data;
        
        // Preload mock data
        ResultSet() {
            data = {
                {1, "Alice", 100.5, true}  // Sample row matching User structure
            };
        }
        
        bool next() { return ++currentRow < data.size(); }
        
        int getInt(int index) {
            int value = std::get<int>(data[currentRow][index-1]);
            std::cout << "Getting INT at [" << index << "]: " << value << "\n";
            return value;
        }
        
        double getDouble(int index) {
            double value = std::get<double>(data[currentRow][index-1]);
            std::cout << "Getting DOUBLE at [" << index << "]: " << value << "\n";
            return value;
        }
        
        std::string getString(int index) {
            std::string value = std::get<std::string>(data[currentRow][index-1]);
            std::cout << "Getting STRING at [" << index << "]: " << value << "\n";
            return value;
        }
        
        bool getBool(int index) {
            bool value = std::get<bool>(data[currentRow][index-1]);
            std::cout << "Getting BOOL at [" << index << "]: " << std::boolalpha << value << "\n";
            return value;
        }
        
        template<typename T>
        void set_value(T& member, int index) {
            if constexpr (std::is_same_v<T, int>) {
                member = getInt(index);
            } else if constexpr (std::is_same_v<T, double>) {
                member = getDouble(index);
            } else if constexpr (std::is_same_v<T, std::string>) {
                member = getString(index);
            } else if constexpr (std::is_same_v<T, bool>) {
                member = getBool(index);
            }
        }
    };
    
    ResultSet* executeQuery() { return new ResultSet(); }
    int executeUpdate() { return 1; }
};

struct Database {
    static Connection* conn;
    
    static void connect(const std::string& user, const std::string& pass, const std::string& db) {
        std::cout << "Connected to mock Oracle DB!\n";
        conn = Connection::createConnection(user, pass, db);
    }
    
    static void disconnect() {
        delete conn;
        std::cout << "Disconnected from mock Oracle DB!\n";
    }
    
    static Statement* createStatement(const std::string& sql) {
        auto stmt = new Statement();
        stmt->sql = sql;
        return stmt;
    }
    
    static void terminateStatement(Statement* stmt) {
        delete stmt;
    }
    
    static void commit() {}
};

Connection* Database::conn = nullptr; 