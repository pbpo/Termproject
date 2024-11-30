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
#include <map>
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "CashManager.hpp"
#include "Bank.hpp"
#include "LanguageSupport.hpp"

WithdrawalTransaction::WithdrawalTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), fee(0) {}

bool WithdrawalTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* atmBank = systemStatus->getBank();
    LanguageSupport* languageSupport = LanguageSupport::getInstance();
    CashManager* cashManager = CashManager::getInstance();

    // 수수료 계산 (REQ 1.8)
    if (account->getBankName() == atmBank->getBankName()) {
        fee = 1000; // 주거래 은행
    } else {
        fee = 2000; // 타행
    }

    int totalAmount = amount + fee;

    // 계좌에서 총 금액 출금 시도
    if (!account->withdraw(totalAmount)) {
        throw InsufficientFundsException(languageSupport->getMessage("insufficient_funds_with_fee"));
    }

    // ATM에서 출금할 금액 분배 시도
    std::map<Denomination, int> dispensedCash;
    if (!cashManager->dispenseCash(amount, dispensedCash)) {
        // 출금 불가능한 경우 롤백
        account->deposit(totalAmount);
        throw InsufficientFundsException(languageSupport->getMessage("cannot_dispense_requested_amount"));
    }

    // 성공 메시지 출력
    std::cout << languageSupport->getMessage("withdrawal_successful") << amount << languageSupport->getMessage("won") << std::endl;
    std::cout << languageSupport->getMessage("fee_deducted") << fee << languageSupport->getMessage("won") << std::endl;

    // 출금된 지폐 내역 출력
    std::cout << languageSupport->getMessage("dispensed_bills") << std::endl;
    for (const auto& [bill, count] : dispensedCash) {
        if (count > 0) {
            std::cout << static_cast<int>(bill) << languageSupport->getMessage("won") << ": " << count << languageSupport->getMessage("bills") << std::endl;
        }
    }

    return true;
}

void WithdrawalTransaction::rollback() {
    LanguageSupport* languageSupport = LanguageSupport::getInstance();
    std::cout << languageSupport->getMessage("rollback_initiated") << std::endl;
    account->deposit(amount + fee);
}

void WithdrawalTransaction::printDetails() const {
    LanguageSupport* languageSupport = LanguageSupport::getInstance();
    std::cout << languageSupport->getMessage("withdrawal_transaction_details") << std::endl;
    std::cout << languageSupport->getMessage("transaction_id") << ": " << transactionID << std::endl;
    std::cout << languageSupport->getMessage("amount") << ": " << amount << languageSupport->getMessage("won") << std::endl;
    std::cout << languageSupport->getMessage("fee") << ": " << fee << languageSupport->getMessage("won") << std::endl;
    std::cout << languageSupport->getMessage("account_number") << ": " << account->getAccountNumber() << std::endl;
}