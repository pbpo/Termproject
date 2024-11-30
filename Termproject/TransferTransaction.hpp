//
//  TransferTransaction.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#pragma once

#include "ITransaction.hpp"
#include "Account.hpp"
#include "Utility.hpp"
#include <memory>

class TransferTransaction : public ITransaction {
private:
    std::shared_ptr<Account> sourceAccount;
    std::shared_ptr<Account> destinationAccount;
    TransferType transferType;
    int fee;

public:
    TransferTransaction(std::shared_ptr<Bank> primaryBank,CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& sourceAccount, const std::shared_ptr<Account>& destinationAccount, TransferType transferType, const std::string& cardNumber);

    bool execute() override;
    void rollback() override;
    void printDetails() const override;
    std::string getTransactionType() const override {
        return "Transfer";
    }

    int getFee() const { return fee; }
};
