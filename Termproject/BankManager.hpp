#pragma once

#include <map>
#include <string>
#include "Bank.hpp"

class BankManager {
private:
    // Map of bank names to Bank pointers
    std::map<std::string, Bank*> banks;

    // Singleton instance
    static BankManager* instance;

    // Private constructor for singleton
    BankManager() {}

public:
    // Get the singleton instance
    static BankManager* getInstance();

    // Add a bank
    void addBank(Bank* bank);

    // Get a bank by name
    Bank* getBank(const std::string& bankName);

    // Get all banks
    const std::map<std::string, Bank*>& getAllBanks() const;
};