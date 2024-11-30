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
    BankManager* manager = BankManager::getInstance();

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


    // If SINGLE, select a bank
    std::shared_ptr<Bank> primaryBank = nullptr;
    if (atmType == ATMType::SINGLE) {
        while (true) {
            auto bankNameVariant = InputHandler::getInput("Enter the bank name for the Single Bank ATM: ", InputType::STRING);
            try {
                std::string bankName = std::get<std::string>(bankNameVariant);

                // Get the bank from BankManager
                Bank* rawBank = BankManager::getInstance()->getBank(bankName);
                if (rawBank) {
                    primaryBank = std::shared_ptr<Bank>(rawBank, [](Bank*) {}); // Create a shared_ptr without deleting rawBank
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
    }

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
    ATM atm(serialNumber, atmType, manager, primaryBank, isBilingual);

    if (atmType == ATMType::SINGLE && primaryBank) {
               std::cout << "Single Bank ATM created for bank: " << primaryBank->getBankName() << std::endl;
    }
    else {
               std::cout << "Multi-Bank ATM created." << std::endl;
    }

    // Set ATM and Bank in SystemStatus
    SystemStatus::getInstance()->setATM(&atm);

    // Start the program
    atm.startSession();
};
