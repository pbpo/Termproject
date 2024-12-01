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
#include "LanguageSupport.hpp"

TransferTransaction::TransferTransaction(std::string primaryBank,CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& sourceAccount, const std::shared_ptr<Account>& destinationAccount, TransferType transferType, const std::string& cardNumber)
    : ITransaction(primaryBank, cashManager, transactionID, amount, cardNumber), sourceAccount(sourceAccount), destinationAccount(destinationAccount), transferType(transferType), fee(0) {languageSupport = LanguageSupport::getInstance();}
bool TransferTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    if (transferType == TransferType::ACCOUNT) {
        // Fee calculation
        if (sourceAccount->getBankName() == destinationAccount->getBankName()) {
            if (sourceAccount->getBankName() == primaryBank) {
                fee = 2000; // Between primary banks
            } else {
                fee = 4000; // Between non-primary banks
            }
        } else {
            if (sourceAccount->getBankName() == globalBank->getBankName() ||
                destinationAccount->getBankName() == primaryBank) {
                fee = 3000; // Between primary and non-primary bank
            } else {
                fee = 4000; // Between non-primary banks
            }
        }

        int totalAmount = amount + fee;
        if (sourceAccount->withdraw(totalAmount)) {
            destinationAccount->deposit(amount);
            std::cout << languageSupport->getMessage("Transferred KRW ")<< amount << languageSupport->getMessage(" from account ") << sourceAccount->getAccountNumber()
                      << languageSupport->getMessage(" to account ") << destinationAccount->getAccountNumber() << "." << std::endl;
            return true;
        } else {
            throw InsufficientFundsException();
        }
    } else if (transferType == TransferType::CASH) {
        fee = 1000; // Cash transfer fee

        std::map<Denomination, int> insertedCash;
        int totalInserted = 0;
        int totalBills = 0;

        std::cout << languageSupport->getMessage("Please insert cash by specifying the number of bills for each denomination.") << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                auto countVariant = InputHandler::getInput("Number of KRW " + std::to_string(denomPair.second) + " bills: ", InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << languageSupport->getMessage("Invalid count. Please enter a non-negative number.") << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("Invalid input. Please enter a number.") << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }
         amount = totalInserted;
         std::map<Denomination, int> feeCasher;
int totalFeeInserted = 0;

        std::cout << languageSupport->getMessage("insert_fee_cash_instruction") << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                std::string prompt = "KRW " + std::to_string(denomPair.second) + " " + languageSupport->getMessage("bill_count_prompt");
                auto countVariant = InputHandler::getInput(prompt, InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << languageSupport->getMessage("negative_bill_count") << std::endl;
                        continue;
                    }
                    break;
                }
                catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            feeCasher[denomPair.first] = count;
            totalFeeInserted += denomPair.second * count;
        }

        // Verify fee amount and handle excess
        if (totalFeeInserted < fee) {
            std::cout << languageSupport->getMessage("insufficient_fee_cash") << std::endl;
            // Request additional fee cash
            while (totalFeeInserted < fee) {
                int remainingFee = fee - totalFeeInserted;
                std::cout << languageSupport->getMessage("remaining_fee") << remainingFee << " " << languageSupport->getMessage("currency_unit") << std::endl;
                std::cout << languageSupport->getMessage("insert_additional_fee_cash_instruction") << std::endl;
                for (const auto& denomPair : DENOMINATION_VALUES) {
                    int count = 0;
                    while (true) {
                        std::string prompt = "KRW " + std::to_string(denomPair.second) + " " + languageSupport->getMessage("bill_count_prompt");
                        auto countVariant = InputHandler::getInput(prompt, InputType::INT);
                        try {
                            count = std::get<int>(countVariant);
                            if (count < 0) {
                                std::cout << languageSupport->getMessage("negative_bill_count") << std::endl;
                                continue;
                            }
                            break;
                        }
                        catch (const std::bad_variant_access&) {
                            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                        }
                    }
                    feeCasher[denomPair.first] += count;
                    totalFeeInserted += denomPair.second * count;
                    if (totalFeeInserted >= fee) break;
                }
            }
        }

        // Calculate excess amount
        int excess = totalFeeInserted - fee;
        std::map<Denomination, int> adjustedfeeCasher;
        std::map<Denomination, int> excessfeeCasher;

        if (excess > 0) {
            std::cout << languageSupport->getMessage("fee_cash_overpaid") << excess << " " << languageSupport->getMessage("currency_unit")
                      << " " << languageSupport->getMessage("will_be_returned") << std::endl;

            // Allocate bills for exact fee
            int feeToAdjust = fee;
            for (auto it = DENOMINATION_VALUES.rbegin(); it != DENOMINATION_VALUES.rend(); ++it) {
                Denomination denom = it->first;
                int denomValue = it->second;
                int availableBills = feeCasher[denom];
                int neededBills = std::min(feeToAdjust / denomValue, availableBills);
                if (neededBills > 0) {
                    adjustedfeeCasher[denom] = neededBills;
                    feeToAdjust -= denomValue * neededBills;
                }
                // Remaining bills are excess
                int excessBills = availableBills - neededBills;
                if (excessBills > 0) {
                    excessfeeCasher[denom] = excessBills;
                }
                if (feeToAdjust == 0) break;
            }

            if (feeToAdjust > 0) {
                // Unable to make exact fee amount with provided bills
                
                // Optionally, handle this by requesting additional bills or accepting overpayment
                // For simplicity, we'll deposit the overpayment into the account
                sourceAccount->deposit(feeToAdjust);
               
                // Adjust excess accordingly
                excess -= feeToAdjust;
            }

            // Deposit excess amount into account instead of returning cash
            if (excess > 0) {
                sourceAccount->deposit(excess);
                
            }

            // Accept only the adjusted fee cash into CashManager
            cashManager->acceptCash(adjustedfeeCasher);
            std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;
        }
        else {
            // Exact fee amount inserted
            cashManager->acceptCash(feeCasher);
            std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;
        }
 

   

        // Accept cash
        cashManager->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("Cash accepted.") << std::endl;

        // Deposit amount to destination account
        destinationAccount->deposit(amount);
        std::cout << languageSupport->getMessage("Transferred KRW ") << amount << languageSupport->getMessage(" to account ") << destinationAccount->getAccountNumber() << "." << std::endl;

        return true;
    }

    return false;
}

void TransferTransaction::rollback() {
    if (transferType == TransferType::ACCOUNT) {
        destinationAccount->withdraw(amount);
        sourceAccount->deposit(amount + fee);
    } else if (transferType == TransferType::CASH) {
        
        destinationAccount->withdraw(amount);
        
    }
}

void TransferTransaction::printDetails() const {
    std::cout << "Transfer Transaction [ID: " << transactionID << ", Amount: " << amount
              << ", Fee: " << fee << ", From: " << (sourceAccount ? sourceAccount->getAccountNumber() : "Cash")
              << ", To: " << destinationAccount->getAccountNumber()
              << ", Type: " << (transferType == TransferType::ACCOUNT ? "Account Transfer" : "Cash Transfer") << "]" << std::endl;
}