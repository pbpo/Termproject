#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp"

DepositTransaction::DepositTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
                                       DepositType depositType, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0), feeDeductedFromAccount(false) {
    languageSupport = LanguageSupport::getInstance();
}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // 수수료 계산
    if (account->getBankName() == globalBank->getBankName()) {
        fee = 1000; // 주거래 은행
    } else {
        fee = 2000; // 비주거래 은행
    }

    feeDeductedFromAccount = false; // 수수료가 계좌에서 차감되었는지 여부

    if (depositType == DepositType::CASH) {
        // 현금 입금 처리
        std::map<Denomination, int> insertedCash;
        int totalInserted = 0;
        int totalBills = 0;

        std::cout << languageSupport->getMessage("insert_cash_instruction") << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                std::string prompt = "KRW " + std::to_string(denomPair.second) + languageSupport->getMessage("bill_count_prompt");
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


        // 수수료를 계좌에서 차감 시도
        if (account->getBalance() >= fee) {
            account->withdraw(fee);
            feeDeductedFromAccount = true;
            std::cout << languageSupport->getMessage("fee_deducted_from_account") << fee << languageSupport->getMessage("currency_unit") << std::endl;
        } else {
            // 입금한 금액에서 수수료 차감
            if (amount >= fee) {
                amount -= fee;
                std::cout << languageSupport->getMessage("fee_deducted_from_deposit") << fee << languageSupport->getMessage("currency_unit") << std::endl;
            } else {
                std::cout << languageSupport->getMessage("insufficient_funds_for_fee") << std::endl;
                return false;
            }
        }

        // 현금 수락
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("cash_accepted") << std::endl;

    } else if (depositType == DepositType::CHECK) {
        // 수표 입금 처리
        

        // 수표 금액이 생성자에서 받은 amount와 일치하는지 확인


        // 수수료를 계좌에서 차감 시도
        if (account->getBalance() >= fee) {
            account->withdraw(fee);
            feeDeductedFromAccount = true;
            std::cout << languageSupport->getMessage("fee_deducted_from_account") << fee << languageSupport->getMessage("currency_unit") << std::endl;
        } else {
            // 수표 금액에서 수수료 차감
            if (amount >= fee) {
                amount -= fee;
                std::cout << languageSupport->getMessage("fee_deducted_from_deposit") << fee << languageSupport->getMessage("currency_unit") << std::endl;
            } else {
                std::cout << languageSupport->getMessage("insufficient_funds_for_fee") << std::endl;
                return false;
            }
        }
    }

    // 계좌에 금액 입금
    account->deposit(amount);
    std::cout << languageSupport->getMessage("deposit_success") << amount << languageSupport->getMessage("currency_unit") << ", "
              << languageSupport->getMessage("account_number") << ": " << account->getAccountNumber() << std::endl;

    return true;
}

void DepositTransaction::rollback() {
    try {
        // 입금된 금액 반환
        if (amount > 0 && account->withdraw(amount)) {
            std::cout << languageSupport->getMessage("rollback_success") << amount << languageSupport->getMessage("currency_unit") << std::endl;
        } else {
            std::cout << languageSupport->getMessage("rollback_failed") << std::endl;
        }

        // 수수료 환불 (계좌에서 차감된 경우)
        if (fee > 0 && feeDeductedFromAccount) {
            account->deposit(fee);
            std::cout << languageSupport->getMessage("fee_refunded") << fee << languageSupport->getMessage("currency_unit") << std::endl;
        }
    } catch (const std::exception& e) {
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
