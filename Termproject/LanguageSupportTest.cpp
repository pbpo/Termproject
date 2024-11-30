#include "LanguageSupportTest.hpp"
#include "LanguageSupport.hpp"
#include <iostream>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include "ATM.hpp"
#include "Bank.hpp"
#include "SecurityManager.hpp"
#include "SystemStatus.hpp"
#include "CashManager.hpp"
#include "Utility.hpp"

using namespace std;

// 생성자 구현
// TestGroup 생성자를 실행할 때 구현한 테스트 개수를 입력해주시기 바랍니다
LanguageSupportTest::LanguageSupportTest(int groupNumber) : TestGroup(groupNumber, 2) {}

// 테스트 함수 구현
void LanguageSupportTest::test_1() {
    //REQ8.1) 이중 언어 지원으로 구성된 ATM은 사용자가 선호하는 언어(영어 또는 한국어)를 선택할 수 있는 옵션을 제공해야 합니다.
    initializeSystemWithTestData();



        
    cout << "Running AdminMenuTest - Test 1" << endl;
}

void LanguageSupportTest::test_2() {
    //(REQ8.2) 특정 언어가 선택되면, 모든 메뉴는 선택된 언어로 표시되어야 합니다.
    // 
    // Get the singleton instance of LanguageSupport
    LanguageSupport* languageSupport = LanguageSupport::getInstance();

    // Define test keys for menu messages
    const std::string menuKeys[] = {
        "invalid_serial",
        "withdrawal_successful",
        "session_end"
    };

    // Define expected messages for English
    const std::map<std::string, std::string> expectedEnglish = {
        {"invalid_serial", "Serial number must be a unique 6-digit number."},
        {"withdrawal_successful", "Withdrawal successful."},
        {"session_end", "Thank you for using the ATM. Session ended."}
    };

    // Define expected messages for Korean
    const std::map<std::string, std::string> expectedKorean = {
        {"invalid_serial", "일련 번호는 고유한 6자리 숫자여야 합니다."},
        {"withdrawal_successful", "출금이 완료되었습니다."},
        {"session_end", "ATM을 이용해 주셔서 감사합니다. 세션이 종료되었습니다."}
    };

    // Test with English
    languageSupport->setLanguage(Language::ENGLISH);
    for (const auto& key : menuKeys) {
        std::string actualMessage = languageSupport->getMessage(key);
        assert(actualMessage == expectedEnglish.at(key) &&
            "Message did not match for English.");
    }
    std::cout << "English language menu display test passed!" << std::endl;

    // Test with Korean
    languageSupport->setLanguage(Language::KOREAN);
    for (const auto& key : menuKeys) {
        std::string actualMessage = languageSupport->getMessage(key);
        assert(actualMessage == expectedKorean.at(key) &&
            "Message did not match for Korean.");
    }
    std::cout << "Korean language menu display test passed!" << std::endl;
}

void LanguageSupportTest::initializeSystemWithTestData() {
    // Create multiple banks
    auto kakaoBank = std::make_shared<Bank>("카카오");
    auto shinhanBank = std::make_shared<Bank>("신한");
    auto wooriBank = std::make_shared<Bank>("우리");

    // Test accounts for 카카오
    auto kakaoAccount1 = std::make_shared<Account>("카카오", "Alice", "1", 1000);
    kakaoBank->addAccount(kakaoAccount1);
    SecurityManager::getInstance()->addUser("1", "1", "카카오");

    auto kakaoAccount2 = std::make_shared<Account>("카카오", "Bob", "2", 1500);
    kakaoBank->addAccount(kakaoAccount2);
    SecurityManager::getInstance()->addUser("2", "2", "카카오");

    // Test accounts for 신한
    auto shinhanAccount1 = std::make_shared<Account>("신한", "Charlie", "3", 2000);
    shinhanBank->addAccount(shinhanAccount1);
    SecurityManager::getInstance()->addUser("3", "3", "신한");

    // Test accounts for 우리
    auto wooriAccount1 = std::make_shared<Account>("우리", "David", "4", 2500);
    wooriBank->addAccount(wooriAccount1);
    SecurityManager::getInstance()->addUser("4", "4", "우리");

    // Add some cash to the ATM
    CashManager::getInstance()->addCash(Denomination::KRW_1000, 50);
    CashManager::getInstance()->addCash(Denomination::KRW_5000, 40);
    CashManager::getInstance()->addCash(Denomination::KRW_10000, 30);
    CashManager::getInstance()->addCash(Denomination::KRW_50000, 20);

    // Choose one bank as primary (e.g., 카카오)
    auto primaryBank = kakaoBank;

    // Create and initialize the ATM
    ATM atm("123456", ATMType::MULTI, primaryBank, true); // Multi-bank ATM

    // Set ATM and primary bank in SystemStatus
    SystemStatus::getInstance()->setATM(&atm);
    SystemStatus::getInstance()->setBank(primaryBank.get());

    // Note: Other banks (신한, 우리) are not directly managed by SystemStatus
    // They should be managed in your ATM's logic if needed.

    // Start the ATM session
    atm.startSession();
}

