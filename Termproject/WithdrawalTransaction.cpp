//
//  WithdrawalTransaction.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "WithdrawalTransaction.hpp"
#include <iostream>
#include <memory>
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp"

WithdrawalTransaction::WithdrawalTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), fee(0) {}

bool WithdrawalTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* atmBank = systemStatus->getBank();
    LanguageSupport* languageSupport = LanguageSupport::getInstance();

    // Fee calculation as per REQ 1.8
    if (account->getBankName() == atmBank->getBankName()) {
        fee = 1000; // Primary bank
    } else {
        fee = 2000; // Non-primary bank
    }

    int totalAmount = amount + fee;

    // Attempt to withdraw totalAmount (amount + fee)
    if (account->withdraw(totalAmount)) {
        std::cout << languageSupport->getMessage("withdrawal_successful") << amount << std::endl;
        std::cout << languageSupport->getMessage("fee_deducted") << fee << std::endl;
        return true;
    } else {
        throw InsufficientFundsException(languageSupport->getMessage("insufficient_funds_with_fee"));
    }
}

void WithdrawalTransaction::rollback() {
    account->deposit(amount + fee);
}

void WithdrawalTransaction::printDetails() const {
    LanguageSupport* languageSupport = LanguageSupport::getInstance();
    std::cout << languageSupport->getMessage("withdrawal_transaction_details") << std::endl;
    std::cout << languageSupport->getMessage("transaction_id") << ": " << transactionID << std::endl;
    std::cout << languageSupport->getMessage("amount") << ": " << amount << std::endl;
    std::cout << languageSupport->getMessage("fee") << ": " << fee << std::endl;
    std::cout << languageSupport->getMessage("account_number") << ": " << account->getAccountNumber() << std::endl;
}


