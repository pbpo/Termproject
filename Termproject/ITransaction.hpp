#pragma once

#include <string>
#include "CashManager.hpp"

class ITransaction {
protected:
    std::string transactionID;
    int amount;
    std::string cardNumber;
    CashManager* cashManager;

public:
    ITransaction(CashManager* cashManager, const std::string& transactionID, int amount, const std::string& cardNumber)
        : cashManager(cashManager),transactionID(transactionID), amount(amount), cardNumber(cardNumber) {}

    virtual ~ITransaction() = default;

    virtual bool execute() = 0;
    virtual void rollback() = 0;
    virtual void printDetails() const = 0;

    // Accessors
    const std::string& getTransactionID() const { return transactionID; }
    int getAmount() const { return amount; }
    const std::string& getCardNumber() const { return cardNumber; }

    // Method to return transaction type
    virtual std::string getTransactionType() const = 0;
};

