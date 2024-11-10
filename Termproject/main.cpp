// main.cpp (수정)

// main.cpp (수정)
#include <iostream>
#include <memory>
#include "ATM.hpp"
#include "Bank.hpp"
#include "SecurityManager.hpp"
#include "SystemStatus.hpp"
#include "CashManager.hpp"
#include "InputHandler.hpp"
#include <string>

// 추가된 함수: 계좌 번호 검증
bool isValidAccountNumber(const std::string& accountNumber) {
    if (accountNumber.length() != 12) {
        std::cout << "Account number must be exactly 12 digits. Please try again." << std::endl;
        return false;
    }

    for (char c : accountNumber) {
        if (!isdigit(c)) {
            std::cout << "Account number must contain only digits. Please try again." << std::endl;
            return false;
        }
    }

    return true;
}

int main() {
    // Initialize bank
    std::shared_ptr<Bank> primaryBank = std::make_shared<Bank>("Kakao");

    // Get account information from user
    int numAccounts = 0;
    while (true) {
        auto numAccountsVariant = InputHandler::getInput("Enter number of accounts to create: ", InputType::INT);
        try {
            numAccounts = std::get<int>(numAccountsVariant);
            if (numAccounts <= 0) {
                std::cout << "Number of accounts must be positive." << std::endl;
                continue;
            }
            break;
        } catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a positive integer." << std::endl;
        }
    }

    for (int i = 0; i < numAccounts; ++i) {
        std::string bankName, ownerName, accountNumber, password;
        int initialBalance;

        // 입력 받기: 은행 이름
        bankName = std::get<std::string>(InputHandler::getInput("Enter bank name for account " + std::to_string(i+1) + ": ", InputType::STRING));

        // 입력 받기: 소유자 이름
        ownerName = std::get<std::string>(InputHandler::getInput("Enter owner name for account " + std::to_string(i+1) + ": ", InputType::STRING));

        // 입력 받기: 12자리 계좌 번호 검증
        while (true) {
            accountNumber = std::get<std::string>(InputHandler::getInput("Enter 12-digit account number for account " + std::to_string(i+1) + ": ", InputType::STRING));
            if (isValidAccountNumber(accountNumber)) {
                break;
            }
            // 유효하지 않은 경우, 루프가 계속되어 다시 입력을 요청함
        }

        // 입력 받기: 비밀번호 설정
        while (true) {
            password = std::get<std::string>(InputHandler::getInput("Set a password for account " + std::to_string(i+1) + ": ", InputType::STRING));
            if (password.empty()) {
                std::cout << "Password cannot be empty. Please try again." << std::endl;
                continue;
            }
            // 비밀번호 확인
            std::string confirmPassword = std::get<std::string>(InputHandler::getInput("Confirm password for account " + std::to_string(i+1) + ": ", InputType::STRING));
            if (password != confirmPassword) {
                std::cout << "Passwords do not match. Please try again." << std::endl;
                continue;
            }
            break;
        }

        // 입력 받기: 초기 잔액
        while (true) {
            auto balanceVariant = InputHandler::getInput("Enter initial balance for account " + std::to_string(i+1) + ": ", InputType::INT);
            try {
                initialBalance = std::get<int>(balanceVariant);
                if (initialBalance < 0) {
                    std::cout << "Initial balance cannot be negative." << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }

        // 계좌 생성 및 추가
        auto account = std::make_shared<Account>(bankName, ownerName, accountNumber, initialBalance);
        primaryBank->addAccount(account);

        // 사용자 인증 정보 추가
        SecurityManager::getInstance()->addUser(accountNumber, password, bankName); // 사용자가 설정한 비밀번호 사용
    }

    // Bank deposits cash into the ATM
    std::cout << "Enter initial cash for the ATM:" << std::endl;
    for (const auto& pair : DENOMINATION_VALUES) {
        int count = 0;
        while (true) {
            auto countVariant = InputHandler::getInput("Number of KRW " + std::to_string(pair.second) + " bills: ", InputType::INT);
            try {
                count = std::get<int>(countVariant);
                if (count < 0) {
                    std::cout << "Number of bills cannot be negative." << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a non-negative integer." << std::endl;
            }
        }
        CashManager::getInstance()->addCash(pair.first, count);
    }

    // Initialize ATM
    std::string serialNumber;
    int atmTypeInput;
    bool isBilingual;

    // ATM 시리얼 번호 입력 (문자열로 변경)
    while (true) {
        auto serialVariant = InputHandler::getInput("Enter ATM serial number (6-digit): ", InputType::STRING);
        try {
            serialNumber = std::get<std::string>(serialVariant);
            if (serialNumber.length() != 6) {
                std::cout << "Serial number must be exactly 6 digits. Please try again." << std::endl;
                continue;
            }
            bool allDigits = true;
            for (char c : serialNumber) {
                if (!isdigit(c)) {
                    allDigits = false;
                    break;
                }
            }
            if (!allDigits) {
                std::cout << "Serial number must contain only digits. Please try again." << std::endl;
                continue;
            }
            break;
        } catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a 6-digit number." << std::endl;
        }
    }

    // ATM 타입 입력
    while (true) {
        auto atmTypeVariant = InputHandler::getInput("Select ATM type: 1. Single Bank ATM 2. Multi-Bank ATM: ", InputType::INT);
        try {
            atmTypeInput = std::get<int>(atmTypeVariant);
            if (atmTypeInput != 1 && atmTypeInput != 2) {
                std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
                continue;
            }
            break;
        } catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter 1 or 2." << std::endl;
        }
    }
    ATMType atmType = (atmTypeInput == 1) ? ATMType::SINGLE : ATMType::MULTI;

    // ATM 언어 설정
    while (true) {
        auto bilingualVariant = InputHandler::getInput("Is the ATM bilingual? (1. Yes 2. No): ", InputType::INT);
        try {
            int bilingualInput = std::get<int>(bilingualVariant);
            if (bilingualInput != 1 && bilingualInput != 2) {
                std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
                continue;
            }
            isBilingual = (bilingualInput == 1);
            break;
        } catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter 1 or 2." << std::endl;
        }
    }

    // Create ATM
    ATM atm(serialNumber, atmType, primaryBank, isBilingual);

    // Set ATM and Bank in SystemStatus
    SystemStatus::getInstance()->setATM(&atm);
    SystemStatus::getInstance()->setBank(primaryBank.get());

    // Start the program
    atm.startSession();

    return 0;
}
