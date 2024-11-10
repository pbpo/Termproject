//
//  TransferTransaction.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "TransferTransaction.hpp"
#include <iostream>
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "CashManager.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp" 

TransferTransaction::TransferTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& sourceAccount, const std::shared_ptr<Account>& destinationAccount, TransferType transferType, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), sourceAccount(sourceAccount), destinationAccount(destinationAccount), transferType(transferType), fee(0) {}

bool TransferTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    if (transferType == TransferType::ACCOUNT) {
        // Fee calculation
        if (sourceAccount->getBankName() == destinationAccount->getBankName()) {
            if (sourceAccount->getBankName() == globalBank->getBankName()) {
                fee = 2000; // Between primary banks
            } else {
                fee = 4000; // Between non-primary banks
            }
        } else {
            if (sourceAccount->getBankName() == globalBank->getBankName() ||
                destinationAccount->getBankName() == globalBank->getBankName()) {
                fee = 3000; // Between primary and non-primary bank
            } else {
                fee = 4000; // Between non-primary banks
            }
        }

        int totalAmount = amount + fee;
        if (sourceAccount->withdraw(totalAmount)) {
            destinationAccount->deposit(amount);
            std::cout << "Transferred KRW " << amount << " from account " << sourceAccount->getAccountNumber()
                      << " to account " << destinationAccount->getAccountNumber() << "." << std::endl;
            return true;
        } else {
            throw InsufficientFundsException();
        }
    } else if (transferType == TransferType::CASH) {
        fee = 1000; // Cash transfer fee

        std::map<Denomination, int> insertedCash;
        int totalInserted = 0;
        int totalBills = 0;

        std::cout << "Please insert cash by specifying the number of bills for each denomination." << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                auto countVariant = InputHandler::getInput("Number of KRW " + std::to_string(denomPair.second) + " bills: ", InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << "Invalid count. Please enter a non-negative number." << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }

        if (totalInserted != amount + fee) {
            std::cout << "Inserted cash amount does not match the transfer amount plus fee." << std::endl;
            return false;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << "Exceeded maximum number of cash bills allowed per transaction." << std::endl;
            return false;
        }

        // Accept cash
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << "Cash accepted." << std::endl;

        // Deposit amount to destination account
        destinationAccount->deposit(amount);
        std::cout << "Transferred KRW " << amount << " to account " << destinationAccount->getAccountNumber() << "." << std::endl;

        return true;
    }

    return false;
}

void TransferTransaction::rollback() {
    if (transferType == TransferType::ACCOUNT) {
        destinationAccount->withdraw(amount);
        sourceAccount->deposit(amount + fee);
    } else if (transferType == TransferType::CASH) {
        // For cash transfer, rollback is not practical as cash has been accepted
        std::cout << "Rollback not possible for cash transfers after cash is accepted." << std::endl;
    }
}

void TransferTransaction::printDetails() const {
    std::cout << "Transfer Transaction [ID: " << transactionID << ", Amount: " << amount
              << ", Fee: " << fee << ", From: " << (sourceAccount ? sourceAccount->getAccountNumber() : "Cash")
              << ", To: " << destinationAccount->getAccountNumber()
              << ", Type: " << (transferType == TransferType::ACCOUNT ? "Account Transfer" : "Cash Transfer") << "]" << std::endl;
}
