// Bank.cpp
#include "Bank.hpp"
#include <iostream>
#include "LanguageSupport.hpp"

Bank::Bank(const std::string& bankName) : bankName(bankName) {}

void Bank::addAccount(const std::shared_ptr<Account>& account) {
    accounts[account->getAccountNumber()] = account;
}

std::shared_ptr<Account> Bank::getAccount(const std::string& accountNumber) const {
    auto it = accounts.find(accountNumber);
    if (it != accounts.end()) {
        return it->second;
    }
    return nullptr;
}

//validateCard추가
bool Bank::validateCard(const std::string& cardNumber) const {
    return accounts.find(cardNumber) != accounts.end();
}

const std::map<std::string, std::shared_ptr<Account>>& Bank::getAllAccounts() const {
    return accounts;
}

void Bank::printAllAccounts() const {
    for (const auto& pair : accounts) {
        std::cout << "Account [Bank: " << bankName << ", No: " << pair.second->getAccountNumber()
                  << ", Owner: " << pair.second->getOwnerName()
                  << "] balance: " << pair.second->getBalance() << std::endl;
    }
}

const std::string& Bank::getBankName() const {
    return bankName;
}
