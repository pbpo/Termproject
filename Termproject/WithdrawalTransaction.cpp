//
//  WithdrawalTransaction.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// WithdrawalTransaction.cpp

#include "WithdrawalTransaction.hpp"
#include <iostream>
#include <memory>
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp" 

WithdrawalTransaction::WithdrawalTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), fee(0) {}

bool WithdrawalTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // Fee calculation
    if (account->getBankName() == globalBank->getBankName()) {
        fee = 1000; // Primary bank
    } else {
        fee = 2000; // Non-primary bank
    }

    int totalAmount = amount + fee;

    if (account->withdraw(totalAmount)) {
        return true;
    } else {
        throw InsufficientFundsException();
    }
}

void WithdrawalTransaction::rollback() {
    account->deposit(amount + fee);
}

void WithdrawalTransaction::printDetails() const {
    std::cout << "Withdrawal Transaction [ID: " << transactionID << ", Amount: " << amount
              << ", Fee: " << fee << ", Account: " << account->getAccountNumber() << "]" << std::endl;
}
