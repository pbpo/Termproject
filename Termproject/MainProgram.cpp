#include <iostream>
#include <memory>
#include "MainProgram.hpp"
#include "ATM.hpp"
#include "Bank.hpp"
#include "SecurityManager.hpp"
#include "SystemStatus.hpp"
#include "CashManager.hpp"
#include "InputHandler.hpp"

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
void initializeSystem() {
    // Initialize bank
    auto primaryBank = std::make_shared<Bank>("Primary Bank");

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
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a positive integer." << std::endl;
        }
    }

    for (int i = 0; i < numAccounts; ++i) {
        std::string bankName, ownerName, accountNumber, password;
        int initialBalance;

        // �Է� �ޱ�: ���� �̸�
        bankName = std::get<std::string>(InputHandler::getInput("Enter bank name for account " + std::to_string(i + 1) + ": ", InputType::STRING));

        // �Է� �ޱ�: ������ �̸�
        ownerName = std::get<std::string>(InputHandler::getInput("Enter owner name for account " + std::to_string(i + 1) + ": ", InputType::STRING));

        // �Է� �ޱ�: 12�ڸ� ���� ��ȣ ����
        while (true) {
            accountNumber = std::get<std::string>(InputHandler::getInput("Enter 12-digit account number for account " + std::to_string(i + 1) + ": ", InputType::STRING));
            if (isValidAccountNumber(accountNumber)) {
                break;
            }
            // ��ȿ���� ���� ���, ������ ��ӵǾ� �ٽ� �Է��� ��û��
        }

        // �Է� �ޱ�: ��й�ȣ ����
        while (true) {
            password = std::get<std::string>(InputHandler::getInput("Set a password for account " + std::to_string(i + 1) + ": ", InputType::STRING));
            if (password.empty()) {
                std::cout << "Password cannot be empty. Please try again." << std::endl;
                continue;
            }
            // ��й�ȣ Ȯ��
            std::string confirmPassword = std::get<std::string>(InputHandler::getInput("Confirm password for account " + std::to_string(i + 1) + ": ", InputType::STRING));
            if (password != confirmPassword) {
                std::cout << "Passwords do not match. Please try again." << std::endl;
                continue;
            }
            break;
        }

        // �Է� �ޱ�: �ʱ� �ܾ�
        while (true) {
            auto balanceVariant = InputHandler::getInput("Enter initial balance for account " + std::to_string(i + 1) + ": ", InputType::INT);
            try {
                initialBalance = std::get<int>(balanceVariant);
                if (initialBalance < 0) {
                    std::cout << "Initial balance cannot be negative." << std::endl;
                    continue;
                }
                break;
            }
            catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }

        // ���� ���� �� �߰�
        auto account = std::make_shared<Account>(bankName, ownerName, accountNumber, initialBalance);
        primaryBank->addAccount(account);

        // ����� ���� ���� �߰�
        SecurityManager::getInstance()->addUser(accountNumber, password, bankName); // ����ڰ� ������ ��й�ȣ ���
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
            }
            catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a non-negative integer." << std::endl;
            }
        }
        CashManager::getInstance()->addCash(pair.first, count);
    }

    // Initialize ATM
    std::string serialNumber;
    int atmTypeInput;
    bool isBilingual;

    // ATM �ø��� ��ȣ �Է� (���ڿ��� ����)
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
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a 6-digit number." << std::endl;
        }
    }

    // ATM Ÿ�� �Է�
    while (true) {
        auto atmTypeVariant = InputHandler::getInput("Select ATM type: 1. Single Bank ATM 2. Multi-Bank ATM: ", InputType::INT);
        try {
            atmTypeInput = std::get<int>(atmTypeVariant);
            if (atmTypeInput != 1 && atmTypeInput != 2) {
                std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
                continue;
            }
            break;
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter 1 or 2." << std::endl;
        }
    }
    ATMType atmType = (atmTypeInput == 1) ? ATMType::SINGLE : ATMType::MULTI;

    // ATM ��� ����
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
        }
        catch (const std::bad_variant_access&) {
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
};