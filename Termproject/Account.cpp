// Account.cpp
#include "Account.hpp"

Account::Account(const std::string& bankName, const std::string& ownerName, const std::string& accountNumber, int initialBalance)
    : bankName(bankName), ownerName(ownerName), accountNumber(accountNumber), balance(initialBalance) {
    cardNumber = accountNumber; // Set card number same as account number
}

void Account::deposit(int amount) {
    balance += amount;
}

bool Account::withdraw(int amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

const std::string& Account::getAccountNumber() const {
    return accountNumber;
}

const std::string& Account::getBankName() const {
    return bankName;
}

const std::string& Account::getCardNumber() const {
    return cardNumber;
}

const std::string& Account::getOwnerName() const {
    return ownerName;
}

int Account::getBalance() const {
    return balance;
}

void Account::addTransaction(const std::shared_ptr<ITransaction>& transaction) {
    transactionHistory.push_back(transaction);
}

const std::vector<std::shared_ptr<ITransaction>>& Account::getTransactionHistory() const {
    return transactionHistory;
}
