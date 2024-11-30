// Bank.hpp
#pragma once

#include <string>
#include <map>
#include <memory>
#include "Account.hpp"

class Bank {
private:
    std::string bankName;
    std::map<std::string, std::shared_ptr<Account>> accounts;

public:
    Bank(const std::string& bankName);

    void addAccount(const std::shared_ptr<Account>& account);
    std::shared_ptr<Account> getAccount(const std::string& accountNumber) const;
    bool validateCard(const std::string& cardNumber) const; //validateCard추가
    const std::map<std::string, std::shared_ptr<Account>>& getAllAccounts() const;
    void printAllAccounts() const;
    const std::string& getBankName() const;
};
