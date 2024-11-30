#include "BankManager.hpp"
#include <iostream>

// Initialize the singleton instance to nullptr
BankManager* BankManager::instance = nullptr;

// Get the singleton instance
BankManager* BankManager::getInstance() {
    if (instance == nullptr) {
        instance = new BankManager();
    }
    return instance;
}

// Add a bank to the manager
void BankManager::addBank(Bank* bank) {
    banks[bank->getBankName()] = bank;
}

// Get a bank by name
Bank* BankManager::getBank(const std::string& bankName) {
    auto it = banks.find(bankName);
    if (it != banks.end()) {
        return it->second;
    }
    return nullptr;
}

// Get all banks
const std::map<std::string, Bank*>& BankManager::getAllBanks() const {
    return banks;
}

// Function to get all accounts from all banks
std::map<std::string, std::shared_ptr<Account>> BankManager::getAllAccounts() const {
    std::map<std::string, std::shared_ptr<Account>> allAccounts;

    for (const auto& bankPair : banks) {
        const auto& bankAccounts = bankPair.second->getAllAccounts();
        allAccounts.insert(bankAccounts.begin(), bankAccounts.end());
    }

    return allAccounts;
}

// Function to print all accounts from all banks
void BankManager::printAllAccounts() const {
    for (const auto& bankPair : banks) {
        std::cout << "Bank: " << bankPair.first << std::endl;
        bankPair.second->printAllAccounts();
        std::cout << "---------------------------------" << std::endl;
    }
}
