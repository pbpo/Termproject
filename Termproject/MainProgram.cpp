#include <iostream>
#include <memory>
#include <vector>
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

std::unique_ptr<ATM> createATM(BankManager* manager) {
    std::string serialNumber;
    int atmTypeInput;
    bool isBilingual;

    // Input ATM serial number
    while (true) {
        auto serialVariant = InputHandler::getInput("Enter ATM serial number (6-digit): ", InputType::STRING);
        try {
            serialNumber = std::get<std::string>(serialVariant);
            if (serialNumber.length() != 6) {
                std::cout << "Serial number must be exactly 6 digits. Please try again." << std::endl;
                continue;
            }
            break;
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a 6-digit number." << std::endl;
        }
    }

    // Initialize CashManager with user input
    CashManager* cashManager = new CashManager();
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
        cashManager->addCash(pair.first, count);
    }

    // Input ATM type
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

    // Select bank if SINGLE
    std::shared_ptr<Bank> primaryBank = nullptr;
    
    while (true) {
        auto bankNameVariant = InputHandler::getInput("Enter the bank name for the Main Bank ATM: ", InputType::STRING);
        try {
            std::string bankName = std::get<std::string>(bankNameVariant);
            Bank* rawBank = manager->getBank(bankName);
            if (rawBank) {
                primaryBank = std::shared_ptr<Bank>(rawBank, [](Bank*) {});
                std::cout << "Bank '" << bankName << "' selected for this ATM." << std::endl;
                break;
            }
            else {
                std::cout << "Bank '" << bankName << "' does not exist. Please enter a valid bank name." << std::endl;
            }
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a valid bank name." << std::endl;
        }
    }
    

    // Set bilingual status
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

    // Create and return the ATM
    return std::make_unique<ATM>(serialNumber, atmType, cashManager, primaryBank, isBilingual);
}


void initializeSystem() {
    // Initialize bank
    BankManager* manager = BankManager::getInstance();
    std::vector<ATM> atmList;

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

        // 입력 받기: 은행 이름
        bankName = std::get<std::string>(InputHandler::getInput("Enter bank name for account " + std::to_string(i + 1) + ": ", InputType::STRING));

        // 입력 받기: 소유자 이름
        ownerName = std::get<std::string>(InputHandler::getInput("Enter owner name for account " + std::to_string(i + 1) + ": ", InputType::STRING));

        // 입력 받기: 12자리 계좌 번호 검증
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
        Bank* bank = manager->getBank(bankName);
        if (!bank) {
            // If the bank doesn't exist, create and add it
            manager->addBank(new Bank(bankName));
            bank = manager->getBank(bankName); // Retrieve the newly added bank
        }
        // Add an account to the bank
        if (bank) {
            auto account = std::make_shared<Account>(bankName, ownerName, accountNumber, initialBalance);
            bank->addAccount(account);
        } else {
            std::cerr << "Error: Bank with name '" << bankName << "' could not be found in BankManager." << std::endl;
        }

        // ����� ���� ���� �߰�
        SecurityManager::getInstance()->addUser(accountNumber, password, bankName); // ����ڰ� ������ ��й�ȣ ���
    }

    // Create multiple ATMs
    int numATMs;
    while (true) {
        auto numATMsVariant = InputHandler::getInput("Enter number of ATMs to create: ", InputType::INT);
        try {
            numATMs = std::get<int>(numATMsVariant);
            if (numATMs <= 0) {
                std::cout << "Number of ATMs must be positive." << std::endl;
                continue;
            }
            break;
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a positive integer." << std::endl;
        }
    }

    std::vector<std::unique_ptr<ATM>> atms;
    for (int i = 0; i < numATMs; ++i) {
        std::cout << "Creating ATM " << (i + 1) << " of " << numATMs << ":" << std::endl;
        atms.push_back(createATM(manager));
    }

    // Allow user to select an ATM by serial number
    while (true) {
        std::string selectedSerial;
        auto serialVariant = InputHandler::getInput("Enter the serial number of the ATM to start a session: ", InputType::STRING);
        try {
            selectedSerial = std::get<std::string>(serialVariant);
            auto it = std::find_if(atms.begin(), atms.end(), [&selectedSerial](const std::unique_ptr<ATM>& atm) {
                return atm->getSerialNumber() == selectedSerial;
                });

            if (it != atms.end()) {
                SystemStatus::getInstance()->setATM(it->get());
                it->get()->startSession();
                break;
            }
            else {
                std::cout << "ATM with serial number " << selectedSerial << " not found. Please try again." << std::endl;
            }
        }
        catch (const std::bad_variant_access&) {
            std::cout << "Invalid input. Please enter a valid serial number." << std::endl;
        }
    }
};
