#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp" // LanguageSupport 포함

DepositTransaction::DepositTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
                                       DepositType depositType, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0) {}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();
    LanguageSupport* languageSupport = LanguageSupport::getInstance();

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

        std::cout << languageSupport->getMessage("insert_cash_instruction") << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                std::string prompt = languageSupport->getMessage("enter_bills") + " KRW " + std::to_string(denomPair.second) + " bills: ";
                auto countVariant = InputHandler::getInput(prompt, InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << languageSupport->getMessage("negative_bill_count") << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }



        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << languageSupport->getMessage("max_bills_exceeded") << std::endl;
            return false;
        }

        // Accept cash
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("cash_accepted") << std::endl;
    } else if (depositType == DepositType::CHECK) {
        int checkAmount = 0;
        while (true) {
            std::cout << languageSupport->getMessage("check_deposit_prompt") << " ";
            auto amountVariant = InputHandler::getInput("", InputType::INT);
            try {
                checkAmount = std::get<int>(amountVariant);
                if (checkAmount < 100000) {
                    std::cout << languageSupport->getMessage("min_check_amount") << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            }
        }



        // Fee must be paid in cash
        int totalInserted = 0;
        std::map<Denomination, int> insertedCash;
        int totalBills = 0;

        std::cout << languageSupport->getMessage("insert_fee_cash") << fee << "." << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                std::string prompt = languageSupport->getMessage("enter_bills") + " KRW " + std::to_string(denomPair.second) + " bills: ";
                auto countVariant = InputHandler::getInput(prompt, InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << languageSupport->getMessage("negative_bill_count") << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }

        if (totalInserted < fee) {
            std::cout << languageSupport->getMessage("insufficient_fee_cash") << std::endl;
            return false;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << languageSupport->getMessage("max_bills_exceeded") << std::endl;
            return false;
        }

        // Accept fee cash
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;

        // Handle excess cash
        if (totalInserted > fee) {
            int excess = totalInserted - fee;
            account->deposit(excess);
            std::cout << languageSupport->getMessage("excess_amount_deposited") << excess << " " << languageSupport->getMessage("currency") << std::endl;
        }
    }

    // Deposit amount to account
    account->deposit(amount);
    std::cout << languageSupport->getMessage("deposit_success") << amount << " " << languageSupport->getMessage("currency")
              << " " << languageSupport->getMessage("into_account") << " " << account->getAccountNumber() << "." << std::endl;

    return true;
}

void DepositTransaction::rollback() {
    // Withdraw deposited amount
    try {
        if (account->withdraw(amount)) {
            std::cout << languageSupport->getMessage("rollback_success") << " " << amount << " " << languageSupport->getMessage("currency")
                      << " " << languageSupport->getMessage("from_account") << " " << account->getAccountNumber() << "." << std::endl;
        } else {
            std::cout << languageSupport->getMessage("rollback_failed") << std::endl;
        }

        // Rollback fee is not necessary as fee cash has already been accepted
    } catch (const std::exception& e) {
        std::cout << languageSupport->getMessage("rollback_error") << " " << e.what() << std::endl;
    }
}

void DepositTransaction::printDetails() const {
    std::string type = (depositType == DepositType::CASH) ? languageSupport->getMessage("transaction_type_cash") : languageSupport->getMessage("transaction_type_check");
    std::cout << languageSupport->getMessage("deposit_transaction_details")
              << " [ID: " << transactionID << ", "
              << languageSupport->getMessage("amount") << ": " << amount << ", "
              << languageSupport->getMessage("fee") << ": " << fee << ", "
              << languageSupport->getMessage("account") << ": " << account->getAccountNumber() << ", "
              << languageSupport->getMessage("type") << ": " << type << "]" << std::endl;
}