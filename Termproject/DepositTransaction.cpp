#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"

DepositTransaction::DepositTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
                                       DepositType depositType, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0) {}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // Fee calculation
    if (account->getBankName() == globalBank->getBankName()) {
        fee = 1000; // Primary bank
    } else {
        fee = 2000; // Non-primary bank
    }


    if (depositType == DepositType::CASH) {
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
            std::cout << "Inserted cash amount does not match the deposit amount plus fee." << std::endl;
            return false;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << "Exceeded maximum number of cash bills allowed per transaction." << std::endl;
            return false;
        }

        // Accept cash
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << "Cash accepted." << std::endl;
    } else if (depositType == DepositType::CHECK) {
        int checkAmount = 0;
        while (true) {
            auto amountVariant = InputHandler::getInput("Please enter the check amount: ", InputType::INT);
            try {
                checkAmount = std::get<int>(amountVariant);
                if (checkAmount < 100000) {
                    std::cout << "Minimum check amount is KRW 100,000." << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }

        if (checkAmount != amount) {
            std::cout << "Check amount does not match the deposit amount." << std::endl;
            return false;
        }

        // Fee must be paid in cash
        int totalInserted = 0;
        std::map<Denomination, int> insertedCash;
        int totalBills = 0;

        std::cout << "Please insert cash for the deposit fee of KRW " << fee << "." << std::endl;
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

        if (totalInserted < fee) {
            std::cout << "Insufficient cash inserted for the fee." << std::endl;
            return false;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << "Exceeded maximum number of cash bills allowed per transaction." << std::endl;
            return false;
        }

        // Accept fee cash
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << "Fee cash accepted." << std::endl;

        // Handle excess cash
        if (totalInserted > fee) {
            int excess = totalInserted - fee;
            account->deposit(excess);
            std::cout << "Excess amount of KRW " << excess << " has been deposited into your account." << std::endl;
        }
    }

    // Deposit amount to account
    account->deposit(amount);
    std::cout << "Deposited KRW " << amount << " into account " << account->getAccountNumber() << "." << std::endl;

    return true;
}

void DepositTransaction::rollback() {
    // Withdraw deposited amount
    try {
        if (account->withdraw(amount)) {
            std::cout << "Rollback: KRW " << amount << " has been withdrawn from account " << account->getAccountNumber() << "." << std::endl;
        } else {
            std::cout << "Rollback failed: Unable to withdraw deposited amount." << std::endl;
        }

        // Rollback fee is not necessary as fee cash has already been accepted
    } catch (const std::exception& e) {
        std::cout << "Error during rollback: " << e.what() << std::endl;
    }
}

void DepositTransaction::printDetails() const {
    std::cout << "Deposit Transaction [ID: " << transactionID << ", Amount: " << amount
              << ", Fee: " << fee << ", Account: " << account->getAccountNumber()
              << ", Type: " << (depositType == DepositType::CASH ? "Cash" : "Check") << "]" << std::endl;
}

