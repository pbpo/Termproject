
//
//  LanguageSupport.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// LanguageSupport.cpp
#include "LanguageSupport.hpp"

LanguageSupport* LanguageSupport::instance = nullptr;

LanguageSupport::LanguageSupport() {
    currentLanguage = Language::ENGLISH;

    // Initialize English messages
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
    messagesEnglish["select_language"] = "Select language:\n1. English\n2. Korean";
    messagesEnglish["welcome"] = "Welcome to the ATM.";
    messagesEnglish["enter_card"] = "Please enter your card number:";
    messagesEnglish["invalid_input"] = "Invalid input. Please try again.";
    messagesEnglish["card_invalid"] = "Invalid card. Please try again.";
    messagesEnglish["enter_password"] = "Please enter your password:";
    messagesEnglish["wrong_password"] = "Incorrect password. Please try again.";
    messagesEnglish["max_password_attempts"] = "Maximum password attempts exceeded. Session ending.";
    messagesEnglish["select_transaction"] = "Please select a transaction:";
    messagesEnglish["invalid_choice"] = "Invalid choice. Please try again.";
    messagesEnglish["session_end"] = "Thank you for using the ATM. Session ended.";

    // Additional English message keys
    messagesEnglish["main_menu_options"] = "1. Deposit\n2. Withdrawal\n3. Transfer\n4. Exit";
    messagesEnglish["select_deposit_type"] = "Select deposit type:";
    messagesEnglish["deposit_type_options"] = "1. Cash\n2. Check";
    messagesEnglish["admin_menu"] = "Admin Menu:";
    messagesEnglish["admin_menu_options"] = "1. View Transaction History\n2. Export Transaction History to File\n3. Exit Admin Menu";
    messagesEnglish["all_transaction_history_header"] = "--- All Transaction History ---";
    messagesEnglish["end_transaction_history"] = "--- End of Transaction History ---";
    messagesEnglish["transaction_history_file_header"] = "=== Transaction History ===";
    messagesEnglish["transaction_id"] = "Transaction ID";
    messagesEnglish["card_number"] = "Card Number";
    messagesEnglish["transaction_type"] = "Type";
    messagesEnglish["amount"] = "Amount";
    messagesEnglish["transaction_history_exported"] = "Transaction history exported to";
    messagesEnglish["transaction_history_successfully_exported"] = "Transaction history successfully exported to";
    messagesEnglish["max_check_deposit_exceeded"] = "Maximum check deposit amount exceeded. Deposit cancelled.";
    messagesEnglish["dispensed_cash"] = "Dispensed cash:";
    messagesEnglish["export_file_open_failed"] = "Failed to open file for writing.";
    messagesEnglish["positive_amount_required"] = "Amount must be positive. Please try again.";
    messagesEnglish["max_withdrawals_exceeded"] = "Maximum number of withdrawals per session exceeded.";
    messagesEnglish["atm_serial_number"] = "ATM Serial Number";
    messagesEnglish["transaction_summary_header"] = "--- Session Summary ---";
    messagesEnglish["end_transaction_summary"] = "--- End of Summary ---";
    messagesEnglish["negative_bill_count"] = "Count cannot be negative. Please try again.";
    messagesEnglish["enter_bills"] = "Enter number of bills";
    messagesEnglish["empty_account_number"] = "Account number cannot be empty. Please try again.";
    messagesEnglish["destination_account_not_found"] = "Destination account not found.";
    messagesEnglish["transfer_successful"] = "Transfer successful.";
    messagesEnglish["insert_cash_instruction"] = "Please insert cash as instructed.";
    messagesEnglish["inserted_amount_mismatch"] = "Inserted amount does not match the required amount.";
    messagesEnglish["max_bills_exceeded"] = "Maximum number of bills exceeded. Deposit cancelled.";
    messagesEnglish["cash_accepted"] = "Cash accepted.";
    messagesEnglish["min_check_amount"] = "Minimum check amount is KRW 100,000.";
    messagesEnglish["insert_fee_cash"] = "Please insert KRW "; // Fee will be appended dynamically
    messagesEnglish["insufficient_fee_cash"] = "Insufficient fee cash inserted.";
    messagesEnglish["fee_cash_accepted"] = "Fee cash accepted.";
    messagesEnglish["excess_amount_deposited"] = "Excess amount deposited: KRW ";
    messagesEnglish["deposit_success"] = "Deposit successful.";
    messagesEnglish["currency"] = "KRW";
    messagesEnglish["into_account"] = "into account";
    messagesEnglish["rollback_success"] = "Rollback successful.";
    messagesEnglish["from_account"] = "from account";
    messagesEnglish["rollback_failed"] = "Rollback failed.";
    messagesEnglish["rollback_error"] = "Error during rollback:";
    messagesEnglish["transaction_type_cash"] = "Cash";
    messagesEnglish["transaction_type_check"] = "Check";
    messagesEnglish["deposit_transaction_details"] = "Deposit Transaction Details";
    messagesEnglish["select_transfer_type"] = "Select transfer type:";
    messagesEnglish["transfer_type_options"] = "1. Cash Transfer\n2. Account Transfer";
    messagesEnglish["enter_destination_account"] = "Enter destination account number:";
    messagesEnglish["invalid_withdrawal_amount"] = "Amount must be positive and less than or equal to KRW 500,000. Please try again.";

    // Initialize Korean messages
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
    messagesKorean["select_language"] = "언어를 선택하세요:\n1. 영어\n2. 한국어";
    messagesKorean["welcome"] = "ATM에 오신 것을 환영합니다.";
    messagesKorean["enter_card"] = "카드 번호를 입력하세요:";
    messagesKorean["invalid_input"] = "유효하지 않은 입력입니다. 다시 시도해주세요.";
    messagesKorean["card_invalid"] = "유효하지 않은 카드입니다. 다시 시도해주세요.";
    messagesKorean["enter_password"] = "비밀번호를 입력하세요:";
    messagesKorean["wrong_password"] = "비밀번호가 틀렸습니다. 다시 시도해주세요.";
    messagesKorean["max_password_attempts"] = "비밀번호 입력 허용 횟수를 초과했습니다. 세션을 종료합니다.";
    messagesKorean["select_transaction"] = "거래를 선택하세요:";
    messagesKorean["invalid_choice"] = "잘못된 선택입니다. 다시 시도해주세요.";
    messagesKorean["session_end"] = "ATM을 이용해 주셔서 감사합니다. 세션이 종료되었습니다.";

    // Additional Korean message keys
    messagesKorean["main_menu_options"] = "1. 입금\n2. 출금\n3. 송금\n4. 종료";
    messagesKorean["select_deposit_type"] = "입금 유형을 선택하세요:";
    messagesKorean["deposit_type_options"] = "1. 현금\n2. 수표";
    messagesKorean["admin_menu"] = "관리자 메뉴:";
    messagesKorean["admin_menu_options"] = "1. 거래 내역 보기\n2. 거래 내역 파일로 내보내기\n3. 관리자 메뉴 종료";
    messagesKorean["all_transaction_history_header"] = "--- 모든 거래 내역 ---";
    messagesKorean["end_transaction_history"] = "--- 거래 내역 끝 ---";
    messagesKorean["transaction_history_file_header"] = "=== 거래 내역 ===";
    messagesKorean["transaction_id"] = "거래 ID";
    messagesKorean["card_number"] = "카드 번호";
    messagesKorean["transaction_type"] = "유형";
    messagesKorean["amount"] = "금액";
    messagesKorean["transaction_history_exported"] = "거래 내역이 다음 파일로 내보내졌습니다:";
    messagesKorean["transaction_history_successfully_exported"] = "거래 내역이 성공적으로 다음 파일로 내보내졌습니다:";
    messagesKorean["max_check_deposit_exceeded"] = "수표 입금 최대 금액을 초과했습니다. 입금이 취소되었습니다.";
    messagesKorean["dispensed_cash"] = "지급된 현금:";
    messagesKorean["export_file_open_failed"] = "파일을 열어 쓰기에 실패했습니다.";
    messagesKorean["positive_amount_required"] = "금액은 양수여야 합니다. 다시 시도해주세요.";
    messagesKorean["max_withdrawals_exceeded"] = "세션당 최대 출금 횟수를 초과했습니다.";
    messagesKorean["atm_serial_number"] = "ATM 일련 번호";
    messagesKorean["transaction_summary_header"] = "--- 세션 요약 ---";
    messagesKorean["end_transaction_summary"] = "--- 요약 끝 ---";
    messagesKorean["negative_bill_count"] = "지폐 수는 음수가 될 수 없습니다. 다시 시도해주세요.";
    messagesKorean["enter_bills"] = "지폐 수 입력";
    messagesKorean["empty_account_number"] = "계좌 번호는 비워둘 수 없습니다. 다시 시도해주세요.";
    messagesKorean["destination_account_not_found"] = "목적지 계좌를 찾을 수 없습니다.";
    messagesKorean["transfer_successful"] = "송금이 완료되었습니다.";
    messagesKorean["insert_cash_instruction"] = "지시대로 현금을 삽입하세요.";
    messagesKorean["inserted_amount_mismatch"] = "삽입된 금액이 필요한 금액과 일치하지 않습니다.";
    messagesKorean["max_bills_exceeded"] = "지폐 수가 최대치를 초과했습니다. 입금이 취소되었습니다.";
    messagesKorean["cash_accepted"] = "현금이 접수되었습니다.";
    messagesKorean["min_check_amount"] = "수표의 최소 금액은 KRW 100,000입니다.";
    messagesKorean["insert_fee_cash"] = "수수료로 KRW "; // Fee will be appended dynamically
    messagesKorean["insufficient_fee_cash"] = "수수료 현금이 부족합니다.";
    messagesKorean["fee_cash_accepted"] = "수수료 현금이 접수되었습니다.";
    messagesKorean["excess_amount_deposited"] = "초과 금액이 입금되었습니다: KRW ";
    messagesKorean["deposit_success"] = "입금이 완료되었습니다.";
    messagesKorean["currency"] = "KRW";
    messagesKorean["into_account"] = "계좌로";
    messagesKorean["rollback_success"] = "롤백이 완료되었습니다.";
    messagesKorean["from_account"] = "계좌에서";
    messagesKorean["rollback_failed"] = "롤백에 실패했습니다.";
    messagesKorean["rollback_error"] = "롤백 중 오류 발생:";
    messagesKorean["transaction_type_cash"] = "현금";
    messagesKorean["transaction_type_check"] = "수표";
    messagesKorean["deposit_transaction_details"] = "입금 거래 세부 정보";
    messagesKorean["select_transfer_type"] = "송금 유형을 선택하세요:";
    messagesKorean["transfer_type_options"] = "1. 현금 송금\n2. 계좌 송금";
    messagesKorean["enter_destination_account"] = "목적지 계좌 번호를 입력하세요:";
    messagesKorean["invalid_withdrawal_amount"] = "금액은 양수이고 KRW 500,000 이하이어야 합니다. 다시 시도해주세요.";

    // **Note:** For messages that require dynamic values (like fees), append them during runtime.
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
        auto it = messagesEnglish.find(key);
        if (it != messagesEnglish.end()) {
            return it->second;
        } else {
            return "Message not found for key: " + key;
        }
    } else {
        auto it = messagesKorean.find(key);
        if (it != messagesKorean.end()) {
            return it->second;
        } else {
            return "메시지를 찾을 수 없습니다: " + key;
        }
    }
}