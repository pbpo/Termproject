//
//  SystemStatus.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "SystemStatus.hpp"
#include "ATM.hpp"
#include "Bank.hpp"
#include <iostream>

SystemStatus* SystemStatus::instance = nullptr;

SystemStatus::SystemStatus() : atm(nullptr), bank(nullptr) {}

SystemStatus* SystemStatus::getInstance() {
    if (instance == nullptr) {
        instance = new SystemStatus();
    }
    return instance;
}

void SystemStatus::setATM(ATM* atm) {
    this->atm = atm;
}

void SystemStatus::setBank(Bank* bank) {
    this->bank = bank;
}

void SystemStatus::displayStatus() {
    std::cout << "\n--- System Status ---" << std::endl;
    if (atm) {
        atm->displayCashInventory();
    }
    if (bank) {
        bank->printAllAccounts();
    }
    std::cout << "--- End of Status ---\n" << std::endl;
}
