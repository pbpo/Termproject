#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp"

DepositTransaction::DepositTransaction(CashManager* cashManager, const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
    DepositType depositType, const std::string& cardNumber)
    : ITransaction(cashManager, transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0), feeDeductedFromAccount(false) {
    languageSupport = LanguageSupport::getInstance();
}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // 수수료 계산
    if (account->getBankName() == globalBank->getBankName()) {
        fee = 1000; // 주거래 은행
    }
    else {
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
                }
                catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }

        // 입금 금액을 amount에 반영
        this->amount += totalInserted;

        // 수수료 처리 루프
        while (true) {
            // 수수료를 계좌에서 차감 시도
            if (account->getBalance() >= fee) {
                account->withdraw(fee);
                feeDeductedFromAccount = true;
                std::cout << languageSupport->getMessage("fee_deducted_from_account") << fee << languageSupport->getMessage("currency_unit") << std::endl;
                break;
            }
            else {
                // 입금한 금액에서 수수료 차감
                if (amount >= fee) {
                    amount -= fee;
                    std::cout << languageSupport->getMessage("fee_deducted_from_deposit") << fee << languageSupport->getMessage("currency_unit") << std::endl;
                    break;
                }
                else {
                    std::cout << languageSupport->getMessage("insufficient_funds_for_fee") << std::endl;

                    // 추가 입금 요청
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

                    // 추가 금액을 amount에 반영
                    amount += additionalAmount;
                    std::cout << languageSupport->getMessage("additional_deposit_added") << additionalAmount << languageSupport->getMessage("currency_unit") << std::endl;
                }
            }
        }

        // 현금 수락
        cashManager->acceptCash(insertedCash);
        std::cout << languageSupport->getMessage("cash_accepted") << std::endl;

    }
    else if (depositType == DepositType::CHECK) {
        // 수표 입금 처리

        // 수표 금액이 생성자에서 받은 amount와 일치하는지 확인
        // (이 부분은 실제 수표 검증 로직에 따라 구현해야 합니다.)

        // 수수료를 별도로 현금으로 지불받기
        std::cout << languageSupport->getMessage("check_fee_payment_instruction") << fee << languageSupport->getMessage("currency_unit") << std::endl;

        // 현금으로 수수료 지불받기
        std::map<Denomination, int> feeCash;
        int totalFeeInserted = 0;

        std::cout << languageSupport->getMessage("insert_fee_cash_instruction") << std::endl;
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
                }
                catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            feeCash[denomPair.first] = count;
            totalFeeInserted += denomPair.second * count;
        }

        // 수수료 금액 검증
        if (totalFeeInserted < fee) {
            std::cout << languageSupport->getMessage("insufficient_fee_cash") << std::endl;
            // 추가 수수료 현금 입력 요청
            while (totalFeeInserted < fee) {
                int remainingFee = fee - totalFeeInserted;
                std::cout << languageSupport->getMessage("remaining_fee") << remainingFee << languageSupport->getMessage("currency_unit") << std::endl;
                std::cout << languageSupport->getMessage("insert_additional_fee_cash_instruction") << std::endl;
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

        // 정확한 수수료 금액만 차감
        int excess = totalFeeInserted - fee;
        if (excess > 0) {
            if (excess > 0) {
            // 환불 처리
            std::cout << languageSupport->getMessage("fee_cash_overpaid") << excess << languageSupport->getMessage("currency_unit")
                      << languageSupport->getMessage("will_be_returned") << std::endl;

            // 현금 환불 시도
            bool refundSuccess = cashManager->dispenseCash(excess, feeCash);
            if (refundSuccess) {
                std::cout << languageSupport->getMessage("refund_successful") << excess << languageSupport->getMessage("currency_unit")
                          << languageSupport->getMessage("has_been_returned") << std::endl;
            } else {
                // 환불 실패 시 계좌에 입금
                account->deposit(excess);
                std::cout << languageSupport->getMessage("refund_failed_deposited") << excess << languageSupport->getMessage("currency_unit")
                          << languageSupport->getMessage("deposited_into_account") << std::endl;
            }
        }
        }

        // 수수료 현금 수락
        cashManager->acceptCash(feeCash);
        std::cout << languageSupport->getMessage("fee_cash_accepted") << std::endl;

        // 수수료는 별도로 현금으로 지불받았으므로, 계좌나 입금 금액에서 수수료를 차감할 필요가 없습니다.
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
        }
        else {
            std::cout << languageSupport->getMessage("rollback_failed") << std::endl;
        }

        // 수수료 환불 (계좌에서 차감된 경우)
        if (fee > 0 && feeDeductedFromAccount) {
            account->deposit(fee);
            std::cout << languageSupport->getMessage("fee_refunded") << fee << languageSupport->getMessage("currency_unit") << std::endl;
        }

        // 수표 입금 시 별도로 현금으로 지불한 수수료는 환불 로직을 추가해야 할 수 있습니다.
        // 예: CashManager를 통해 환불 처리
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
