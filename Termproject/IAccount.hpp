#pragma once

#include <string>
#include <vector>
#include <memory>

class ITransaction;

class IAccount {
public:
    virtual ~IAccount() = default;
    virtual const std::string& getAccountNumber() const = 0;
    virtual const std::string& getBankName() const = 0;
    virtual const std::string& getOwnerName() const = 0;
    virtual int getBalance() const = 0;
    virtual void deposit(int amount) = 0;
    virtual bool withdraw(int amount) = 0;
    virtual void addTransaction(const std::shared_ptr<ITransaction>& transaction) = 0;
    virtual void printAccountInfo() const = 0;

    // 추가: 거래 내역 반환
    virtual const std::vector<std::shared_ptr<ITransaction>>& getTransactionHistory() const = 0;

    // 추가: 카드 번호 반환 (TransactionFactory에서 필요)
    virtual const std::string& getCardNumber() const = 0; // 추가
};

