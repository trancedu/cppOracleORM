#include "User.hpp"
#include "UserMapping.hpp"
#include "ORM.hpp"
#include <iostream>

int main() {
    try {
        Database::connect("user", "pass", "mock_db");
        
        User user(1, "Alice", 100.5, true);
        ORM::insertObject(Database::conn, user);
        std::cout << "User inserted (mock)\n";
        
        User loaded = ORM::getObjectById<User>(Database::conn, 1);
        std::cout << "Loaded user: " 
                  << loaded.id << ", " 
                  << loaded.name << ", " 
                  << loaded.balance << ", "
                  << loaded.is_active << "\n";

        Database::disconnect();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
} 