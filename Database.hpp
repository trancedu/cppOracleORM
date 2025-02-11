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
    std::vector<std::variant<int, double, std::string>> params;
    
    void setInt(int index, int value) { params.emplace_back(value); }
    void setDouble(int index, double value) { params.emplace_back(value); }
    void setString(int index, const std::string& value) { params.emplace_back(value); }
    
    struct ResultSet {
        int currentRow = -1;
        std::vector<std::vector<std::variant<int, double, std::string>>> data;
        
        // Preload mock data
        ResultSet() {
            data = {
                {1, "Alice", 100.5}  // Sample row matching User structure
            };
        }
        
        bool next() { return ++currentRow < data.size(); }
        int getInt(int index) { return std::get<int>(data[currentRow][index-1]); }
        double getDouble(int index) { return std::get<double>(data[currentRow][index-1]); }
        std::string getString(int index) { return std::get<std::string>(data[currentRow][index-1]); }
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