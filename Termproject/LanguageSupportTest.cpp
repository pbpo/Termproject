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

// ������ ����
// TestGroup �����ڸ� ������ �� ������ �׽�Ʈ ������ �Է����ֽñ� �ٶ��ϴ�
LanguageSupportTest::LanguageSupportTest(int groupNumber) : TestGroup(groupNumber, 2) {}

// �׽�Ʈ �Լ� ����
void LanguageSupportTest::test_1() {
    //REQ8.1) ���� ��� �������� ������ ATM�� ����ڰ� ��ȣ�ϴ� ���(���� �Ǵ� �ѱ���)�� ������ �� �ִ� �ɼ��� �����ؾ� �մϴ�.
    initializeSystemWithTestData();



        
    cout << "Running AdminMenuTest - Test 1" << endl;
}

void LanguageSupportTest::test_2() {
    //(REQ8.2) Ư�� �� ���õǸ�, ��� �޴��� ���õ� ���� ǥ�õǾ�� �մϴ�.
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
        {"invalid_serial", "�Ϸ� ��ȣ�� ������ 6�ڸ� ���ڿ��� �մϴ�."},
        {"withdrawal_successful", "����� �Ϸ�Ǿ����ϴ�."},
        {"session_end", "ATM�� �̿��� �ּż� �����մϴ�. ������ ����Ǿ����ϴ�."}
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
    auto kakaoBank = std::make_shared<Bank>("īī��");
    auto shinhanBank = std::make_shared<Bank>("����");
    auto wooriBank = std::make_shared<Bank>("�츮");

    // Test accounts for īī��
    auto kakaoAccount1 = std::make_shared<Account>("īī��", "Alice", "1", 1000);
    kakaoBank->addAccount(kakaoAccount1);
    SecurityManager::getInstance()->addUser("1", "1", "īī��");

    auto kakaoAccount2 = std::make_shared<Account>("īī��", "Bob", "2", 1500);
    kakaoBank->addAccount(kakaoAccount2);
    SecurityManager::getInstance()->addUser("2", "2", "īī��");

    // Test accounts for ����
    auto shinhanAccount1 = std::make_shared<Account>("����", "Charlie", "3", 2000);
    shinhanBank->addAccount(shinhanAccount1);
    SecurityManager::getInstance()->addUser("3", "3", "����");

    // Test accounts for �츮
    auto wooriAccount1 = std::make_shared<Account>("�츮", "David", "4", 2500);
    wooriBank->addAccount(wooriAccount1);
    SecurityManager::getInstance()->addUser("4", "4", "�츮");

    // Add some cash to the ATM
    CashManager::getInstance()->addCash(Denomination::KRW_1000, 50);
    CashManager::getInstance()->addCash(Denomination::KRW_5000, 40);
    CashManager::getInstance()->addCash(Denomination::KRW_10000, 30);
    CashManager::getInstance()->addCash(Denomination::KRW_50000, 20);

    // Choose one bank as primary (e.g., īī��)
    auto primaryBank = kakaoBank;

    // Create and initialize the ATM
    ATM atm("123456", ATMType::MULTI, primaryBank, true); // Multi-bank ATM

    // Set ATM and primary bank in SystemStatus
    SystemStatus::getInstance()->setATM(&atm);
    SystemStatus::getInstance()->setBank(primaryBank.get());

    // Note: Other banks (����, �츮) are not directly managed by SystemStatus
    // They should be managed in your ATM's logic if needed.

    // Start the ATM session
    atm.startSession();
}

