//
//  WithdrawalTransaction.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// WithdrawalTransaction.hpp
#pragma once

#include "ITransaction.hpp"
#include "Account.hpp"
#include <memory>

class WithdrawalTransaction : public ITransaction {
private:
    std::shared_ptr<Account> account;
    int fee;

public:
    WithdrawalTransaction(std::shared_ptr<Bank> primaryBank,CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& account, const std::string& cardNumber);

    bool execute() override;
    void rollback() override;
    void printDetails() const override;
    std::string getTransactionType() const override {
        return "Withdrawal";
    }

    int getFee() const { return fee; }
};
