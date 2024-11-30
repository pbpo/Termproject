#pragma once
#include <memory>
#include <string>
#include "CashManager.hpp"
#include "Bank.hpp"

class ITransaction {
protected:
    std::string transactionID;
    int amount;
    std::string cardNumber;
    CashManager* cashManager;
    std::shared_ptr<Bank> primaryBank;

public:
    ITransaction(std::shared_ptr<Bank> primaryBank, CashManager* cashManager, const std::string& transactionID, int amount, const std::string& cardNumber)
        : primaryBank(primaryBank),cashManager(cashManager),transactionID(transactionID), amount(amount), cardNumber(cardNumber) {}

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

