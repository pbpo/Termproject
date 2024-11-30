#pragma once

#include "ITransaction.hpp"
#include "Account.hpp"
#include "Utility.hpp"
#include <memory>
#include "LanguageSupport.hpp"

class DepositTransaction : public ITransaction {
private:
    std::shared_ptr<Account> account;
    DepositType depositType;
    int fee;
    LanguageSupport* languageSupport;
    bool feeDeductedFromAccount;

public:
    // amount 매개변수를 추가한 생성자
    DepositTransaction(CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
        DepositType depositType, const std::string& cardNumber);

    bool execute() override;
    void rollback() override;
    void printDetails() const override;
    std::string getTransactionType() const override {
        return "Deposit";
    }

    int getFee() const { return fee; }
};
