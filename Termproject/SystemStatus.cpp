//
//  SystemStatus.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "SystemStatus.hpp"
#include <iostream>

SystemStatus* SystemStatus::instance = nullptr;

SystemStatus::SystemStatus() : atm(nullptr) {}

SystemStatus* SystemStatus::getInstance() {
    if (instance == nullptr) {
        instance = new SystemStatus();
    }
    return instance;
}

void SystemStatus::setATM(ATM* atm) {
    this->atm = atm;
}

ATM* SystemStatus::getATM() const {
    return atm;
}

// setBank 메서드 수정: Bank*를 직접 전달
void SystemStatus::setBank(Bank* bank) {
    if (bank) {
        BankManager::getInstance()->addBank(bank);
    }
}

// getBank 메서드 수정: Bank*를 반환
Bank* SystemStatus::getBank() const {
    auto banks = BankManager::getInstance()->getAllBanks();
    if (!banks.empty()) {
        // 첫 번째 은행을 반환합니다.
        return banks.begin()->second;
    }
    return nullptr;
}

void SystemStatus::displayStatus() {
    std::cout << "\n--- System Status ---" << std::endl;

    if (atm) {
        atm->displayCashInventory();
    }

    // BankManager의 싱글톤 인스턴스를 가져옵니다.
    BankManager* bankManager = BankManager::getInstance();
    const auto& banks = bankManager->getAllBanks();

    // 모든 은행의 계좌 정보를 출력합니다.
    for (const auto& pair : banks) {
        Bank* bank = pair.second;
        bank->printAllAccounts();
    }

    std::cout << "--- End of Status ---\n" << std::endl;
}