//
//  SecurityManager.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#pragma once

#include <string>
#include <map>

class SecurityManager {
private:
    std::map<std::string, std::string> userCredentials; // cardNumber -> password
    std::map<std::string, std::string> userBankMapping; // cardNumber -> bankName
    std::string adminPassword;

    SecurityManager();
    static SecurityManager* instance;

public:
    static SecurityManager* getInstance();
    bool validateCard(const std::string& cardNumber, const std::string& bankName = "");
    bool authenticateUser(const std::string& cardNumber, const std::string& password);
    bool authenticateAdmin(const std::string& password);
    void addUser(const std::string& cardNumber, const std::string& password, const std::string& bankName);
};
