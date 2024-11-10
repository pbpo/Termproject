//
//  SecurityManager.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "SecurityManager.hpp"

SecurityManager* SecurityManager::instance = nullptr;

SecurityManager::SecurityManager() {
    adminPassword = "admin123"; // Set admin password
}

SecurityManager* SecurityManager::getInstance() {
    if (instance == nullptr) {
        instance = new SecurityManager();
    }
    return instance;
}

bool SecurityManager::validateCard(const std::string& cardNumber, const std::string& bankName) {
    if (userCredentials.find(cardNumber) != userCredentials.end()) {
        if (bankName.empty() || userBankMapping[cardNumber] == bankName) {
            return true;
        }
    }
    return false;
}

bool SecurityManager::authenticateUser(const std::string& cardNumber, const std::string& password) {
    if (userCredentials.find(cardNumber) != userCredentials.end()) {
        return userCredentials[cardNumber] == password;
    }
    return false;
}

bool SecurityManager::authenticateAdmin(const std::string& password) {
    return adminPassword == password;
}

void SecurityManager::addUser(const std::string& cardNumber, const std::string& password, const std::string& bankName) {
    userCredentials[cardNumber] = password;
    userBankMapping[cardNumber] = bankName;
}
