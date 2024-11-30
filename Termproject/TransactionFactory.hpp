#pragma once

#include <memory>
#include "ITransaction.hpp"
#include "DepositTransaction.hpp"
#include "WithdrawalTransaction.hpp"
#include "TransferTransaction.hpp"
#include "Account.hpp"
#include <random>
#include "Utility.hpp"

class TransactionFactory {
public:
    static std::string generateTransactionID() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(100000, 999999);
        return "TXN" + std::to_string(dis(gen));
    }

    static std::shared_ptr<ITransaction> createDepositTransaction(std::string primaryBank,CashManager* cashManager, int amount, std::shared_ptr<Account> account, DepositType depositType, const std::string& cardNumber) {
        return std::make_shared<DepositTransaction>(
            primaryBank,
            cashManager,
            generateTransactionID(),
            amount,
            account,
            depositType,
            cardNumber
        );
    }

    static std::shared_ptr<ITransaction> createWithdrawalTransaction(std::string primaryBank,CashManager* cashManager, int amount, std::shared_ptr<Account> account, const std::string& cardNumber) {
        return std::make_shared<WithdrawalTransaction>(
            primaryBank,
            cashManager,
            generateTransactionID(),
            amount,
            account,
            cardNumber
        );
    }

    static std::shared_ptr<ITransaction> createTransferTransaction(std::string primaryBank,CashManager* cashManager, int amount, std::shared_ptr<Account> sourceAccount, std::shared_ptr<Account> destinationAccount, TransferType transferType, const std::string& cardNumber) {
        return std::make_shared<TransferTransaction>(
            primaryBank,
            cashManager,
            generateTransactionID(),
            amount,
            sourceAccount,
            destinationAccount,
            transferType,
            cardNumber
        );
    }
};

