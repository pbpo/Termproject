#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp"



DepositTransaction::DepositTransaction(std::string primaryBank,CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
    DepositType depositType, const std::string& cardNumber)
    : ITransaction(primaryBank,cashManager, transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0), feeDeductedFromAccount(false),totalBills(0) {
    languageSupport = LanguageSupport::getInstance();
}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // Calculate fee
    if (account->getBankName() == primaryBank) {
        fee = 1000; // Primary bank
    }
    else {
        fee = 2000; // Secondary bank
    }

    feeDeductedFromAccount = false; // Indicates if fee was deducted from account

    if (depositType == DepositType::CASH) {
        // Handle cash deposit
        std::map<Denomination, int> insertedCash;
        int totalInserted = 0;
        totalBills = 0;

        std::cout << languageSupport->getMessage("insert_cash_instruction") << std::endl;
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
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }

        // Update deposit amount
        this->amount += totalInserted;

        // Fee handling loop
        while (true) {
            // Attempt to deduct fee from account
            if (account->getBalance() >= fee) {
                account->withdraw(fee);
                feeDeductedFromAccount = true;
                std::cout << languageSupport->getMessage("fee_deducted_from_account") << fee << " " << languageSupport->getMessage("currency_unit") << std::endl;
                break;
            }
            else {
                // Attempt to deduct fee from deposit amount
                if (amount >= fee) {
                    amount -= fee;
                    std::cout << languageSupport->getMessage("fee_deducted_from_deposit") << fee << " " << languageSupport->getMessage("currency_unit") << std::endl;
                    break;
                }
                else {
                    std::cout << languageSupport->getMessage("insufficient_funds_for_fee") << std::endl;

                    // Request additional deposit
                    std::cout << languageSupport->getMessage("prompt_additional_deposit") << std::endl;
                    int additionalAmount = 0;
                    while (true) {
                        auto additionalInput = InputHandler::getInput(languageSupport->getMessage("enter_additional_amount"), InputType::INT);
                        try {
                            additionalAmount = std::get<int>(additionalInput);
                            if (additionalAmount <= 0) {
                                std::cout << languageSupport->getMessage("invalid_additional_amount") << std::endl;
                                continue;
                            }
                            break;
                        }
                        catch (const std::bad_variant_access&) {
                            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                        }
                    }

                    // Update deposit amount
                    amount += additionalAmount;
                    std::cout << languageSupport->getMessage("additional_deposit_added") << additionalAmount << " " << languageSupport->getMessage("currency_unit") << std::endl;
                }
            }
        }

        // Accept cash into CashManager
        cashManager->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("cash_accepted") << std::endl;

    }
    else if (depositType == DepositType::CHECK) {
        // Handle check deposit

        // Assume amount is already set from handleDeposit()
        // No need to prompt for deposit amount again

        // Prompt user to pay fee in cash
        std::cout << languageSupport->getMessage("check_fee_payment_instruction") << fee << " " << languageSupport->getMessage("currency_unit") << std::endl;

        // Collect fee cash from user
        std::map<Denomination, int> feeCash;
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
            feeCash[denomPair.first] = count;
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
                    feeCash[denomPair.first] += count;
                    totalFeeInserted += denomPair.second * count;
                    if (totalFeeInserted >= fee) break;
                }
            }
        }

        // Calculate excess amount
        int excess = totalFeeInserted - fee;
        std::map<Denomination, int> adjustedFeeCash;
        std::map<Denomination, int> excessFeeCash;
cashManager->acceptCash(feeCash);

        if (excess > 0) {
            std::cout << languageSupport->getMessage("fee_cash_overpaid") << excess << " " << languageSupport->getMessage("currency_unit")
                      << " " << languageSupport->getMessage("will_be_returned") << std::endl;

            // Allocate bills for exact fee
            int feeToAdjust = fee;
            for (auto it = DENOMINATION_VALUES.rbegin(); it != DENOMINATION_VALUES.rend(); ++it) {
                Denomination denom = it->first;
                int denomValue = it->second;
                int availableBills = feeCash[denom];
                int neededBills = std::min(feeToAdjust / denomValue, availableBills);
                if (neededBills > 0) {
                    adjustedFeeCash[denom] = neededBills;
                    feeToAdjust -= denomValue * neededBills;
                }
                // Remaining bills are excess
                int excessBills = availableBills - neededBills;
                if (excessBills > 0) {
                    excessFeeCash[denom] = excessBills;
                }
                if (feeToAdjust == 0) break;
            }

            if (feeToAdjust > 0) {
                // Unable to make exact fee amount with provided bills
                
                // Optionally, handle this by requesting additional bills or accepting overpayment
                // For simplicity, we'll deposit the overpayment into the account
                account->deposit(feeToAdjust);
               
                // Adjust excess accordingly
                excess -= feeToAdjust;
            }

            // Deposit excess amount into account instead of returning cash
            if (excess > 0) {
                account->deposit(excess);
                
            }

            // Accept only the adjusted fee cash into CashManager
            
            
            std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;
        }
        else {
            // Exact fee amount inserted
            
            std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;
        }

        // No need to adjust the account or amount further as fee is handled separately
    }

    // Deposit the remaining amount into the account
    account->deposit(amount);
    std::cout << languageSupport->getMessage("deposit_success") << amount << " " << languageSupport->getMessage("currency_unit") << ", "
              << languageSupport->getMessage("account_number") << ": " << account->getAccountNumber() << std::endl;

    return true;
}
int DepositTransaction::total() const{
    return totalBills;
}
void DepositTransaction::rollback() {
    try {
        // Return deposited amount
        if (amount > 0 && account->withdraw(amount)) {
            std::cout << languageSupport->getMessage("rollback_success") << amount << " " << languageSupport->getMessage("currency_unit") << std::endl;
        }
        else {
            std::cout << languageSupport->getMessage("rollback_failed") << std::endl;
        }

        // Refund fee if it was deducted from the account
        if (fee > 0 && feeDeductedFromAccount) {
            account->deposit(fee);
            std::cout << languageSupport->getMessage("fee_refunded") << fee << " " << languageSupport->getMessage("currency_unit") << std::endl;
        }

        // Handle refund for check deposit fee by depositing back into account
        if (depositType == DepositType::CHECK) {
            // Deposit fee back into account
            account->deposit(fee);
            std::cout << languageSupport->getMessage("fee_refunded_into_account") << fee << " " << languageSupport->getMessage("currency_unit") << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << languageSupport->getMessage("rollback_error") << e.what() << std::endl;
    }
}

void DepositTransaction::printDetails() const {
    std::string type = (depositType == DepositType::CASH) ? languageSupport->getMessage("cash") : languageSupport->getMessage("check");
    std::cout << languageSupport->getMessage("deposit_transaction_details")
              << " [ID: " << transactionID << ", "
              << languageSupport->getMessage("amount") << ": " << amount << ", "
              << languageSupport->getMessage("fee") << ": " << fee << ", "
              << languageSupport->getMessage("account_number") << ": " << account->getAccountNumber() << ", "
              << languageSupport->getMessage("type") << ": " << type << "]" << std::endl;
}