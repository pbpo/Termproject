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
    messagesEnglish["welcome"] = "Welcome to the ATM!";
    messagesEnglish["enter_card"] = "Please enter your card number:";
    messagesEnglish["enter_password"] = "Please enter your password:";
    messagesEnglish["wrong_password"] = "Incorrect password.";
    messagesEnglish["card_invalid"] = "Invalid card.";
    messagesEnglish["session_end"] = "Session ended. Thank you!";
    messagesEnglish["select_language"] = "Please select a language: 1. English 2. Korean";
    messagesEnglish["select_transaction"] = "Please select a transaction type:";
    messagesEnglish["max_password_attempts"] = "Maximum password attempts exceeded. Session ending.";
    messagesEnglish["invalid_input"] = "Invalid input. Please try again.";
    messagesEnglish["invalid_choice"] = "Invalid choice.";

    messagesKorean["welcome"] = "ATM에 오신 것을 환영합니다!";
    messagesKorean["enter_card"] = "카드 번호를 입력해주세요:";
    messagesKorean["enter_password"] = "비밀번호를 입력해주세요:";
    messagesKorean["wrong_password"] = "비밀번호가 틀렸습니다.";
    messagesKorean["card_invalid"] = "유효하지 않은 카드입니다.";
    messagesKorean["session_end"] = "세션이 종료되었습니다. 감사합니다!";
    messagesKorean["select_language"] = "언어를 선택해주세요: 1. 영어 2. 한국어";
    messagesKorean["select_transaction"] = "거래 유형을 선택해주세요:";
    messagesKorean["max_password_attempts"] = "비밀번호 입력 횟수를 초과했습니다. 세션을 종료합니다.";
    messagesKorean["invalid_input"] = "유효하지 않은 입력입니다. 다시 시도해주세요.";
    messagesKorean["invalid_choice"] = "잘못된 선택입니다.";
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
