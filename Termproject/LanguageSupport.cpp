//
//  LanguageSupport.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "LanguageSupport.hpp"

LanguageSupport* LanguageSupport::instance = nullptr;

LanguageSupport::LanguageSupport() {
    currentLanguage = Language::ENGLISH;

    // Initialize messages
messagesEnglish["invalid_serial"] = "Serial number must be a unique 6-digit number.";
messagesEnglish["duplicate_serial"] = "Serial number already assigned to another ATM.";
messagesEnglish["denomination_prompt"] = "Enter the number of bills for each denomination:";
messagesEnglish["invalid_denomination"] = "Invalid denomination entered.";
messagesEnglish["no_cash_entered"] = "No cash entered. Deposit cancelled.";
messagesEnglish["deposit_successful"] = "Deposit successful. Total amount deposited: KRW ";
messagesEnglish["deposit_failed"] = "Deposit failed.";
messagesEnglish["check_deposit_prompt"] = "Enter check amount:";
messagesEnglish["check_deposit_successful"] = "Check deposit successful. Amount: KRW ";
messagesEnglish["check_deposit_failed"] = "Check deposit failed.";
messagesEnglish["withdrawal_amount_prompt"] = "Enter amount to withdraw (multiples of KRW 1000):";
messagesEnglish["withdrawal_successful"] = "Withdrawal successful.";
messagesEnglish["withdrawal_failed"] = "Withdrawal failed.";
messagesEnglish["amount_must_be_multiple"] = "Amount must be in multiples of KRW 1000. Please try again.";
messagesEnglish["atm_insufficient_cash"] = "ATM cannot dispense the requested amount with available denominations. Transaction rolled back.";
messagesEnglish["invalid_serial_number"] = "Serial number must be a 6-digit number.";

messagesKorean["invalid_serial"] = "일련 번호는 고유한 6자리 숫자여야 합니다.";
messagesKorean["duplicate_serial"] = "일련 번호가 이미 다른 ATM에 할당되어 있습니다.";
messagesKorean["denomination_prompt"] = "각 지폐에 대한 개수를 입력하세요:";
messagesKorean["invalid_denomination"] = "잘못된 지폐 단위가 입력되었습니다.";
messagesKorean["no_cash_entered"] = "현금이 입력되지 않았습니다. 입금이 취소되었습니다.";
messagesKorean["deposit_successful"] = "입금이 완료되었습니다. 총 입금 금액: KRW ";
messagesKorean["deposit_failed"] = "입금에 실패하였습니다.";
messagesKorean["check_deposit_prompt"] = "수표 금액을 입력하세요:";
messagesKorean["check_deposit_successful"] = "수표 입금이 완료되었습니다. 금액: KRW ";
messagesKorean["check_deposit_failed"] = "수표 입금에 실패하였습니다.";
messagesKorean["withdrawal_amount_prompt"] = "출금할 금액을 입력하세요 (KRW 1000 단위):";
messagesKorean["withdrawal_successful"] = "출금이 완료되었습니다.";
messagesKorean["withdrawal_failed"] = "출금에 실패하였습니다.";
messagesKorean["amount_must_be_multiple"] = "금액은 KRW 1000의 배수여야 합니다. 다시 시도해주세요.";
messagesKorean["atm_insufficient_cash"] = "ATM에서 사용 가능한 지폐로 요청한 금액을 지급할 수 없습니다. 거래가 취소되었습니다.";
messagesKorean["invalid_serial_number"] = "일련 번호는 6자리 숫자여야 합니다.";

}

LanguageSupport* LanguageSupport::getInstance() {
    if (instance == nullptr) {
        instance = new LanguageSupport();
    }
    return instance;
}

void LanguageSupport::setLanguage(Language language) {
    currentLanguage = language;
}

std::string LanguageSupport::getMessage(const std::string& key) {
    if (currentLanguage == Language::ENGLISH) {
        return messagesEnglish[key];
    } else {
        return messagesKorean[key];
    }
}
