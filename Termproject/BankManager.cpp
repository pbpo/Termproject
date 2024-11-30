#include "BankManager.hpp"

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