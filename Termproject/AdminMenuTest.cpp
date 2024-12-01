#include "AdminMenuTest.hpp"
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
#include <iostream>

using namespace std;

// 생성자 구현
// TestGroup 생성자를 실행할 때 구현한 테스트 개수를 입력해주시기 바랍니다
AdminMenuTest::AdminMenuTest(int groupNumber) : TestGroup(groupNumber, 11) {}

// 테스트 함수 구현
void AdminMenuTest::test_1() {
    //(REQ7.1) 관리자가 관리자 카드를 삽입하여 세션을 시작하면(시스템 설정의 REQ 참조), ATM은 "거래 내역" 메뉴만 표시합니다.
    initializeSystemWithTestData();

}

void AdminMenuTest::test_2() {
    //(REQ7.2) "거래 내역" 메뉴를 선택하면, ATM은 시스템 시작 이후 모든 사용자의 모든 거래 정보를 표시합니다.

       //거래 ID, 카드 번호, 거래 유형, 금액, 기타 거래별 정보
       //각 거래는 서로 다른 유형의 정보를 포함할 수 있으므로, 적절히 표시해야 합니다(예: 입금 거래에는 이체 거래에 있는 출처 계좌 정보가 포함되지 않습니다).

    cout << "Running AdminMenuTest - Test 2" << endl;
}

void AdminMenuTest::test_3() {
    //o(REQ7.3) "거래 내역" 정보는 외부 파일(예: txt 파일)로 출력되어야 합니다.
    cout << "Running AdminMenuTest - Test 3" << endl;
}

void AdminMenuTest::initializeSystemWithTestData() {

    BankManager* manager = BankManager::getInstance();

    manager->addBank(new Bank("카카오"));
    manager->addBank(new Bank("신한"));
    manager->addBank(new Bank("우리"));


    Bank* kakaoBank = manager->getBank("카카오");
    Bank* shinhanBank = manager->getBank("신한");
    Bank* wooriBank = manager->getBank("우리");



    // Test accounts for 카카오
    auto kakaoAccount1 = std::make_shared<Account>("카카오", "Alice", "1", 0);
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
    CashManager* cashManager = new CashManager();
    cashManager->addCash(Denomination::KRW_1000, 0);
    cashManager->addCash(Denomination::KRW_5000, 0);
    cashManager->addCash(Denomination::KRW_10000, 0);
    cashManager->addCash(Denomination::KRW_50000, 0);

    // Choose one bank as primary (e.g., 카카오)
    std::shared_ptr<Bank> primaryBank(kakaoBank, [](Bank*) {
        // Do nothing to avoid double deletion
        });


    // Create and initialize the ATM
    ATM atm("123456", ATMType::MULTI, cashManager, primaryBank, true); // Multi-bank ATM

    // Set ATM and primary bank in SystemStatus
    SystemStatus::getInstance()->setATM(&atm);

    // Note: Other banks (신한, 우리) are not directly managed by SystemStatus
    // They should be managed in your ATM's logic if needed.

    // Start the ATM session
    atm.startSession();
}
