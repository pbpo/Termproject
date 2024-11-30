
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
    messages["invalid_serial"]["English"] = "Serial number must be a unique 6-digit number.";
    messages["invalid_serial"]["Korean"] = "일련 번호는 고유한 6자리 숫자여야 합니다.";

    messages["duplicate_serial"]["English"] = "Serial number already assigned to another ATM.";
    messages["duplicate_serial"]["Korean"] = "일련 번호가 이미 다른 ATM에 할당되어 있습니다.";

    messages["denomination_prompt"]["English"] = "Enter the number of bills for each denomination:";
    messages["denomination_prompt"]["Korean"] = "각 지폐에 대한 개수를 입력하세요:";

    messages["invalid_denomination"]["English"] = "Invalid denomination entered.";
    messages["invalid_denomination"]["Korean"] = "잘못된 지폐 단위가 입력되었습니다.";

    messages["no_cash_entered"]["English"] = "No cash entered. Deposit cancelled.";
    messages["no_cash_entered"]["Korean"] = "현금이 입력되지 않았습니다. 입금이 취소되었습니다.";

    messages["deposit_successful"]["English"] = "Deposit successful. Total amount deposited: KRW ";
    messages["deposit_successful"]["Korean"] = "입금이 완료되었습니다. 총 입금 금액: KRW ";

    messages["deposit_failed"]["English"] = "Deposit failed.";
    messages["deposit_failed"]["Korean"] = "입금에 실패하였습니다.";

    messages["check_deposit_prompt"]["English"] = "Enter check amount:";
    messages["check_deposit_prompt"]["Korean"] = "수표 금액을 입력하세요:";

    messages["check_deposit_successful"]["English"] = "Check deposit successful. Amount: KRW ";
    messages["check_deposit_successful"]["Korean"] = "수표 입금이 완료되었습니다. 금액: KRW ";

    messages["check_deposit_failed"]["English"] = "Check deposit failed.";
    messages["check_deposit_failed"]["Korean"] = "수표 입금에 실패하였습니다.";

    messages["withdrawal_amount_prompt"]["English"] = "Enter amount to withdraw (multiples of KRW 1000):";
    messages["withdrawal_amount_prompt"]["Korean"] = "출금할 금액을 입력하세요 (KRW 1000 단위):";

    messages["withdrawal_successful"]["English"] = "Withdrawal successful.";
    messages["withdrawal_successful"]["Korean"] = "출금이 완료되었습니다.";

    messages["withdrawal_failed"]["English"] = "Withdrawal failed.";
    messages["withdrawal_failed"]["Korean"] = "출금에 실패하였습니다.";

    messages["amount_must_be_multiple"]["English"] = "Amount must be in multiples of KRW 1000. Please try again.";
    messages["amount_must_be_multiple"]["Korean"] = "금액은 KRW 1000의 배수여야 합니다. 다시 시도해주세요.";

    messages["atm_insufficient_cash"]["English"] = "ATM cannot dispense the requested amount with available denominations. Transaction rolled back.";
    messages["atm_insufficient_cash"]["Korean"] = "ATM에서 사용 가능한 지폐로 요청한 금액을 지급할 수 없습니다. 거래가 취소되었습니다.";

    messages["invalid_serial_number"]["English"] = "Serial number must be a 6-digit number.";
    messages["invalid_serial_number"]["Korean"] = "일련 번호는 6자리 숫자여야 합니다.";

    messages["select_language"]["English"] = "Select language:\n1. English\n2. Korean";
    messages["select_language"]["Korean"] = "언어를 선택하세요:\n1. 영어\n2. 한국어";

    messages["welcome"]["English"] = "Welcome to the ATM.";
    messages["welcome"]["Korean"] = "ATM에 오신 것을 환영합니다.";

    messages["enter_card"]["English"] = "Please enter your card number:";
    messages["enter_card"]["Korean"] = "카드 번호를 입력하세요:";

    messages["invalid_input"]["English"] = "Invalid input. Please try again.";
    messages["invalid_input"]["Korean"] = "유효하지 않은 입력입니다. 다시 시도해주세요.";

    messages["card_invalid"]["English"] = "Invalid card. Please try again.";
    messages["card_invalid"]["Korean"] = "유효하지 않은 카드입니다. 다시 시도해주세요.";

    messages["enter_password"]["English"] = "Please enter your password:";
    messages["enter_password"]["Korean"] = "비밀번호를 입력하세요:";

    messages["wrong_password"]["English"] = "Incorrect password. Please try again.";
    messages["wrong_password"]["Korean"] = "비밀번호가 틀렸습니다. 다시 시도해주세요.";

    messages["max_password_attempts"]["English"] = "Maximum password attempts exceeded. Session ending.";
    messages["max_password_attempts"]["Korean"] = "비밀번호 입력 허용 횟수를 초과했습니다. 세션을 종료합니다.";

    messages["select_transaction"]["English"] = "Please select a transaction:";
    messages["select_transaction"]["Korean"] = "거래를 선택하세요:";

    messages["invalid_choice"]["English"] = "Invalid choice. Please try again.";
    messages["invalid_choice"]["Korean"] = "잘못된 선택입니다. 다시 시도해주세요.";

    messages["session_end"]["English"] = "Thank you for using the ATM. Session ended.";
    messages["session_end"]["Korean"] = "ATM을 이용해 주셔서 감사합니다. 세션이 종료되었습니다.";

    messages["main_menu_options"]["English"] = "1. Deposit\n2. Withdrawal\n3. Transfer\n4. Exit";
    messages["main_menu_options"]["Korean"] = "1. 입금\n2. 출금\n3. 송금\n4. 종료";

    messages["select_deposit_type"]["English"] = "Select deposit type:";
    messages["select_deposit_type"]["Korean"] = "입금 유형을 선택하세요:";

    messages["deposit_type_options"]["English"] = "1. Cash\n2. Check";
    messages["deposit_type_options"]["Korean"] = "1. 현금\n2. 수표";

    messages["admin_menu"]["English"] = "Admin Menu:";
    messages["admin_menu"]["Korean"] = "관리자 메뉴:";

    messages["admin_menu_options"]["English"] = "1. View Transaction History\n2. Export Transaction History to File\n3. Exit Admin Menu";
    messages["admin_menu_options"]["Korean"] = "1. 거래 내역 보기\n2. 거래 내역 파일로 내보내기\n3. 관리자 메뉴 종료";

    messages["all_transaction_history_header"]["English"] = "--- All Transaction History ---";
    messages["all_transaction_history_header"]["Korean"] = "--- 모든 거래 내역 ---";

    messages["end_transaction_history"]["English"] = "--- End of Transaction History ---";
    messages["end_transaction_history"]["Korean"] = "--- 거래 내역 끝 ---";

    messages["transaction_history_file_header"]["English"] = "=== Transaction History ===";
    messages["transaction_history_file_header"]["Korean"] = "=== 거래 내역 ===";

    messages["transaction_id"]["English"] = "Transaction ID";
    messages["transaction_id"]["Korean"] = "거래 ID";

    messages["card_number"]["English"] = "Card Number";
    messages["card_number"]["Korean"] = "카드 번호";

    messages["transaction_type"]["English"] = "Type";
    messages["transaction_type"]["Korean"] = "유형";

    messages["amount"]["English"] = "Amount";
    messages["amount"]["Korean"] = "금액";

    messages["transaction_history_exported"]["English"] = "Transaction history exported to";
    messages["transaction_history_exported"]["Korean"] = "거래 내역이 다음 파일로 내보내졌습니다:";

    messages["transaction_history_successfully_exported"]["English"] = "Transaction history successfully exported to";
    messages["transaction_history_successfully_exported"]["Korean"] = "거래 내역이 성공적으로 다음 파일로 내보내졌습니다:";

    messages["max_check_deposit_exceeded"]["English"] = "Maximum check deposit amount exceeded. Deposit cancelled.";
    messages["max_check_deposit_exceeded"]["Korean"] = "수표 입금 최대 금액을 초과했습니다. 입금이 취소되었습니다.";

    messages["dispensed_cash"]["English"] = "Dispensed cash:";
    messages["dispensed_cash"]["Korean"] = "지급된 현금:";

    messages["export_file_open_failed"]["English"] = "Failed to open file for writing.";
    messages["export_file_open_failed"]["Korean"] = "파일을 열어 쓰기에 실패했습니다.";

    messages["positive_amount_required"]["English"] = "Amount must be positive. Please try again.";
    messages["positive_amount_required"]["Korean"] = "금액은 양수여야 합니다. 다시 시도해주세요.";

    messages["max_withdrawals_exceeded"]["English"] = "Maximum number of withdrawals per session exceeded.";
    messages["max_withdrawals_exceeded"]["Korean"] = "세션당 최대 출금 횟수를 초과했습니다.";

    messages["atm_serial_number"]["English"] = "ATM Serial Number";
    messages["atm_serial_number"]["Korean"] = "ATM 일련 번호";

    messages["transaction_summary_header"]["English"] = "--- Session Summary ---";
    messages["transaction_summary_header"]["Korean"] = "--- 세션 요약 ---";

    messages["end_transaction_summary"]["English"] = "--- End of Summary ---";
    messages["end_transaction_summary"]["Korean"] = "--- 요약 끝 ---";

    messages["negative_bill_count"]["English"] = "Count cannot be negative. Please try again.";
    messages["negative_bill_count"]["Korean"] = "지폐 수는 음수가 될 수 없습니다. 다시 시도해주세요.";

    messages["enter_bills"]["English"] = "Enter number of bills:";
    messages["enter_bills"]["Korean"] = "지폐 수를 입력하세요:";

    messages["empty_account_number"]["English"] = "Account number cannot be empty. Please try again.";
    messages["empty_account_number"]["Korean"] = "계좌 번호는 비워둘 수 없습니다. 다시 시도해주세요.";

    messages["destination_account_not_found"]["English"] = "Destination account not found.";
    messages["destination_account_not_found"]["Korean"] = "목적지 계좌를 찾을 수 없습니다.";

    messages["transfer_successful"]["English"] = "Transfer successful.";
    messages["transfer_successful"]["Korean"] = "송금이 완료되었습니다.";

    messages["insert_cash_instruction"]["English"] = "Please insert cash as instructed.";
    messages["insert_cash_instruction"]["Korean"] = "지시대로 현금을 삽입하세요.";

    messages["inserted_amount_mismatch"]["English"] = "Inserted amount does not match the required amount.";
    messages["inserted_amount_mismatch"]["Korean"] = "삽입된 금액이 필요한 금액과 일치하지 않습니다.";

    messages["max_bills_exceeded"]["English"] = "Maximum number of bills exceeded. Deposit cancelled.";
    messages["max_bills_exceeded"]["Korean"] = "지폐 수가 최대치를 초과했습니다. 입금이 취소되었습니다.";

    messages["cash_accepted"]["English"] = "Cash accepted.";
    messages["cash_accepted"]["Korean"] = "현금이 접수되었습니다.";

    messages["min_check_amount"]["English"] = "Minimum check amount is KRW 100,000.";
    messages["min_check_amount"]["Korean"] = "수표의 최소 금액은 KRW 100,000입니다.";

    messages["insert_fee_cash"]["English"] = "Please insert KRW "; // Fee will be appended dynamically
    messages["insert_fee_cash"]["Korean"] = "KRW을(를) 삽입해주세요: "; // 수수료가 동적으로 추가됩니다.

    messages["insufficient_fee_cash"]["English"] = "Insufficient fee cash inserted.";
    messages["insufficient_fee_cash"]["Korean"] = "수수료 현금이 부족합니다.";

    messages["fee_cash_accepted"]["English"] = "Fee cash accepted.";
    messages["fee_cash_accepted"]["Korean"] = "수수료 현금이 접수되었습니다.";

    messages["excess_amount_deposited"]["English"] = "Excess amount deposited: KRW ";
    messages["excess_amount_deposited"]["Korean"] = "초과 금액이 입금되었습니다: KRW ";

    messages["deposit_success"]["English"] = "Deposit successful.";
    messages["deposit_success"]["Korean"] = "입금이 완료되었습니다.";

    messages["currency"]["English"] = "KRW";
    messages["currency"]["Korean"] = "KRW";

    messages["into_account"]["English"] = "into account";
    messages["into_account"]["Korean"] = "계좌로";

    messages["rollback_success"]["English"] = "Rollback successful.";
    messages["rollback_success"]["Korean"] = "롤백이 완료되었습니다.";

    messages["from_account"]["English"] = "from account";
    messages["from_account"]["Korean"] = "계좌에서";

    messages["rollback_failed"]["English"] = "Rollback failed.";
    messages["rollback_failed"]["Korean"] = "롤백에 실패했습니다.";

    messages["rollback_error"]["English"] = "Error during rollback:";
    messages["rollback_error"]["Korean"] = "롤백 중 오류 발생:";

    messages["transaction_type_cash"]["English"] = "Cash";
    messages["transaction_type_cash"]["Korean"] = "현금";

    messages["transaction_type_check"]["English"] = "Check";
    messages["transaction_type_check"]["Korean"] = "수표";

    messages["deposit_transaction_details"]["English"] = "Deposit Transaction Details";
    messages["deposit_transaction_details"]["Korean"] = "입금 거래 세부 정보";

    messages["select_transfer_type"]["English"] = "Select transfer type:";
    messages["select_transfer_type"]["Korean"] = "송금 유형을 선택하세요:";

    messages["transfer_type_options"]["English"] = "1. Cash Transfer\n2. Account Transfer";
    messages["transfer_type_options"]["Korean"] = "1. 현금 송금\n2. 계좌 송금";

    messages["enter_destination_account"]["English"] = "Enter destination account number:";
    messages["enter_destination_account"]["Korean"] = "목적지 계좌 번호를 입력하세요:";

    messages["invalid_withdrawal_amount"]["English"] = "Amount must be positive and less than or equal to KRW 500,000. Please try again.";
    messages["invalid_withdrawal_amount"]["Korean"] = "금액은 양수이고 KRW 500,000 이하이어야 합니다. 다시 시도해주세요.";

   messages["withdrawal_transaction_details"]["English"] = "Withdrawal Transaction Details";
    messages["withdrawal_transaction_details"]["Korean"] = "출금 거래 세부 정보";

    messages["fee_deducted"]["English"] = "Fee deducted: KRW ";
    messages["fee_deducted"]["Korean"] = "수수료 차감: KRW ";

    messages["insufficient_funds_with_fee"]["English"] = "Insufficient funds to withdraw requested amount with fee.";
    messages["insufficient_funds_with_fee"]["Korean"] = "요청된 금액과 수수료를 출금하기에 잔액이 부족합니다.";

    messages["fee_refunded"]["English"] = "Fee refunded: KRW ";
    messages["fee_refunded"]["Korean"] = "수수료 환불: KRW ";

    messages["withdrawal_transaction_details"]["English"] = "Withdrawal Transaction Details";

    messages["withdrawal_transaction_details"]["Korean"] = "출금 거래 세부 정보";

    messages["transaction_id"]["English"] = "Transaction ID";
    messages["transaction_id"]["Korean"] = "거래 ID";

    messages["amount"]["English"] = "Amount";
    messages["amount"]["Korean"] = "금액";

    messages["fee"]["English"] = "Fee";
    messages["fee"]["Korean"] = "수수료";

    messages["account_number"]["English"] = "Account Number";

    messages["account_number"]["Korean"] = "계좌 번호";

    messages["withdrawal_successful"]["English"] = "Withdrawal successful. Amount: KRW ";
    messages["withdrawal_successful"]["Korean"] = "출금이 완료되었습니다. 금액: KRW ";

    messages["rollback_success"]["English"] = "Rollback successful.";
    messages["rollback_success"]["Korean"] = "롤백이 완료되었습니다.";

    messages["rollback_failed"]["English"] = "Rollback failed.";
    messages["rollback_failed"]["Korean"] = "롤백에 실패했습니다.";

    messages["rollback_error"]["English"] = "Error during rollback:";
    messages["rollback_error"]["Korean"] = "롤백 중 오류 발생:";

    messages["withdrawal_transaction_details"]["English"] = "Withdrawal Transaction Details";
    messages["withdrawal_transaction_details"]["Korean"] = "출금 거래 세부 정보";

    messages["transaction_id"]["English"] = "Transaction ID";
    messages["transaction_id"]["Korean"] = "거래 ID";

   messages["cash"]["English"] = "Cash";
    messages["cash"]["Korean"] = "현금";

    messages["check"]["English"] = "Check";
    messages["check"]["Korean"] = "수표";

    messages["enter_deposit_amount"]["English"] = "Enter deposit amount:";
    messages["enter_deposit_amount"]["Korean"] = "입금 금액을 입력하세요:";

    messages["invalid_deposit_amount"]["English"] = "Invalid deposit amount. Please try again.";
    messages["invalid_deposit_amount"]["Korean"] = "유효하지 않은 입금 금액입니다. 다시 시도해주세요.";

    messages["deposit_transaction_details"]["English"] = "Deposit Transaction Details";
    messages["deposit_transaction_details"]["Korean"] = "입금 거래 세부 정보";

    messages["deposit_type"]["English"] = "Type";
    messages["deposit_type"]["Korean"] = "유형";

 

    

 

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
    std::string langKey = (currentLanguage == Language::ENGLISH) ? "English" : "Korean";

    // Check if the key exists
    if (messages.find(key) != messages.end()) {
        // Check if the language exists for the key
        if (messages[key].find(langKey) != messages[key].end()) {
            return messages[key][langKey];
        }
        return (currentLanguage == Language::ENGLISH)
            ? "Message not available in selected language."
            : "선택한 언어로 메시지를 사용할 수 없습니다.";
    }
    return (currentLanguage == Language::ENGLISH)
        ? "Message not found for key: " + key
        : "키에 대한 메시지를 찾을 수 없습니다: " + key;
}
