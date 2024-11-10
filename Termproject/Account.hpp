#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ITransaction.hpp"

class Account {
private:
    std::string bankName;
    std::string ownerName;
    std::string accountNumber;
    int balance;
    std::string cardNumber;
    std::vector<std::shared_ptr<ITransaction>> transactionHistory;

public:
    Account(const std::string& bankName, const std::string& ownerName, const std::string& accountNumber, int initialBalance);

    void deposit(int amount);
    bool withdraw(int amount);
    const std::string& getAccountNumber() const;
    const std::string& getBankName() const;
    const std::string& getCardNumber() const;
    const std::string& getOwnerName() const;
    int getBalance() const;
    void addTransaction(const std::shared_ptr<ITransaction>& transaction);
    const std::vector<std::shared_ptr<ITransaction>>& getTransactionHistory() const;
};

