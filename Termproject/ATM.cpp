//
// ATM.cpp
// Termproject
//
// Created by cho on 10/20/24.
//

#include "ATM.hpp"
#include "DepositTransaction.hpp"
#include "WithdrawalTransaction.hpp"
#include "TransferTransaction.hpp"
#include "Exceptions.hpp"
#include "InputHandler.hpp"
#include "SystemStatus.hpp"
#include "TransactionFactory.hpp"
#include "BankManager.hpp"
#include "SecurityManager.hpp"
#include "Utility.hpp" // Language enum을 위해 포함
#include <iostream>
#include <variant>
#include <fstream>
#include <algorithm>

// Definition of the static member variable
std::set<std::string> ATM::assignedSerialNumbers;

// Constructor for Single Bank ATM
ATM::ATM(const std::string& serialNumber, ATMType atmType, std::shared_ptr<Bank> primaryBank, bool isBilingual)
    : atmType(atmType), primaryBank(primaryBank),
      languageSupport(LanguageSupport::getInstance()),
      cashManager(CashManager::getInstance()),
      securityManager(SecurityManager::getInstance()),
      wrongPasswordAttempts(0),
      isAdminSession(false) {

    // Validate serial number (REQ 1.1)
    if (serialNumber.length() != 6 || !std::all_of(serialNumber.begin(), serialNumber.end(), ::isdigit)) {
        throw InvalidSerialNumberException(languageSupport->getMessage("invalid_serial_number"));
    }
    if (assignedSerialNumbers.find(serialNumber) != assignedSerialNumbers.end()) {
        throw DuplicateSerialNumberException(languageSupport->getMessage("duplicate_serial"));
    }
    this->serialNumber = serialNumber;
    assignedSerialNumbers.insert(serialNumber);

    // Set ATM and Bank in system status
    SystemStatus::getInstance()->setATM(this);
    SystemStatus::getInstance()->setBank(primaryBank.get());

    // Language support setup
    this->isBilingual = isBilingual;
    if (isBilingual) {
        int choice = 0;
        std::cout << languageSupport->getMessage("select_language") << std::endl;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input_default_language") << std::endl;
            choice = 1;
        }

        if (choice == 1) {
            languageSupport->setLanguage(Language::ENGLISH);
        } else if (choice == 2) {
            languageSupport->setLanguage(Language::KOREAN);
        } else {
            languageSupport->setLanguage(Language::ENGLISH);
        }
    }
}

// Constructor for Multi-Bank ATM
ATM::ATM(const std::string& serialNumber, ATMType atmType, bool isBilingual)
        : atmType(atmType), primaryBank(nullptr),
          languageSupport(LanguageSupport::getInstance()),
          cashManager(CashManager::getInstance()),
          securityManager(SecurityManager::getInstance()),
          wrongPasswordAttempts(0),
          isAdminSession(false) 
    {

    // Validate serial number (REQ 1.1)
    if (serialNumber.length() != 6 || !std::all_of(serialNumber.begin(), serialNumber.end(), ::isdigit)) {
        throw InvalidSerialNumberException(languageSupport->getMessage("invalid_serial_number"));
    }
    if (assignedSerialNumbers.find(serialNumber) != assignedSerialNumbers.end()) {
        throw DuplicateSerialNumberException(languageSupport->getMessage("duplicate_serial"));
    }
    this->serialNumber = serialNumber;
    assignedSerialNumbers.insert(serialNumber);

    // Set ATM in system status
    SystemStatus::getInstance()->setATM(this);
    // Multi-Bank ATM does not have a primary bank

    // Language support setup
    this->isBilingual = isBilingual;
    if (isBilingual) {
        int choice = 0;
        std::cout << languageSupport->getMessage("select_language") << std::endl;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input_default_language") << std::endl;
            choice = 1;
        }

        if (choice == 1) {
            languageSupport->setLanguage(Language::ENGLISH);
        } else if (choice == 2) {
            languageSupport->setLanguage(Language::KOREAN);
        } else {
            languageSupport->setLanguage(Language::ENGLISH);
        }
    }
}

// Start Session
void ATM::startSession() {
    wrongPasswordAttempts = 0;
    while (true) {
        std::cout << languageSupport->getMessage("welcome") << std::endl;

        auto cardNumberVariant = InputHandler::getInput(languageSupport->getMessage("enter_card") + "\n", InputType::STRING);
        std::string cardNumber;
        try {
            cardNumber = std::get<std::string>(cardNumberVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        try {
            bool isValidCard = false;
            if (atmType == ATMType::SINGLE) {
                isValidCard = securityManager->validateCard(cardNumber, primaryBank->getBankName());
            } else {
                // For Multi-Bank ATM, any bank's card is valid
                isValidCard = securityManager->validateCard(cardNumber);
            }

            if (!isValidCard) {
                throw InvalidCardException(languageSupport->getMessage("card_invalid"));
            }

            // Admin card check
            if (cardNumber == "000000") {
                isAdminSession = true;
                handleAdminMenu();
                endSession();
                continue;
            }

            // User authentication
            while (wrongPasswordAttempts < 3) {
                auto passwordVariant = InputHandler::getInput(languageSupport->getMessage("enter_password") + "\n", InputType::STRING);
                std::string password;
                try {
                    password = std::get<std::string>(passwordVariant);
                } catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                    wrongPasswordAttempts++;
                    continue;
                }

                if (securityManager->authenticateUser(cardNumber, password)) {
                    // Authentication successful
                    if (atmType == ATMType::SINGLE) {
                        currentAccount = primaryBank->getAccount(cardNumber);
                    } else {
                        // For Multi-Bank ATM, search all banks
                        currentAccount = nullptr;
                        auto banks = BankManager::getInstance()->getAllBanks();
                        for (const auto& pair : banks) {
                            currentAccount = pair.second->getAccount(cardNumber);
                            if (currentAccount) {
                                break;
                            }
                        }
                    }

                    if (!currentAccount) {
                        std::cout << languageSupport->getMessage("account_not_found") << std::endl;
                        break;
                    }
                    currentCardNumber = cardNumber;
                    showMainMenu();
                    break;
                } else {
                    wrongPasswordAttempts++;
                    std::cout << languageSupport->getMessage("wrong_password") << std::endl;
                }
            }

            if (wrongPasswordAttempts >= 3) {
                throw WrongPasswordException(languageSupport->getMessage("max_password_attempts"));
            }
        } catch (const ATMException& e) {
            std::cout << e.what() << std::endl;

            endSession();
            wrongPasswordAttempts = 0;
            continue;
        }
    }
}

// Add session transaction
void ATM::addSessionTransaction(const std::shared_ptr<ITransaction>& transaction) {
    sessionTransactions.push_back(transaction);
}

// Handle Admin Menu
void ATM::handleAdminMenu() {
    while (true) {
        std::cout << languageSupport->getMessage("admin_menu") << std::endl;
        std::cout << languageSupport->getMessage("admin_menu_options") << std::endl;

        int choice = 0;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                displayAllTransactionHistory();
                break;
            case 2:
                exportTransactionHistoryToFile("transaction_history.txt");
                std::cout << languageSupport->getMessage("transaction_history_exported") << std::endl;
                break;
            case 3:
                return;
            default:
                std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
                break;
        }
    }
}

// Display all transaction history
void ATM::displayAllTransactionHistory() const {
    std::cout << "\n" << languageSupport->getMessage("all_transaction_history_header") << std::endl;

    if (atmType == ATMType::SINGLE) {
        // Single Bank ATM
        for (const auto& pair : primaryBank->getAllAccounts()) {
            auto account = pair.second;
            for (const auto& transaction : account->getTransactionHistory()) {
                transaction->printDetails();
            }
        }
    } else {
        // Multi-Bank ATM
        auto banks = BankManager::getInstance()->getAllBanks();
        for (const auto& bankPair : banks) {
            auto bank = bankPair.second;
            for (const auto& accountPair : bank->getAllAccounts()) {
                auto account = accountPair.second;
                for (const auto& transaction : account->getTransactionHistory()) {
                    transaction->printDetails();
                }
            }
        }
    }

    std::cout << languageSupport->getMessage("end_transaction_history") << std::endl;
}

// Export transaction history to file
void ATM::exportTransactionHistoryToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << languageSupport->getMessage("export_file_open_failed") << std::endl;
        return;
    }

    outFile << languageSupport->getMessage("transaction_history_file_header") << "\n";

    if (atmType == ATMType::SINGLE) {
        // Single Bank ATM
        for (const auto& pair : primaryBank->getAllAccounts()) {
            auto account = pair.second;
            for (const auto& transaction : account->getTransactionHistory()) {
                outFile << languageSupport->getMessage("transaction_id") << ": " << transaction->getTransactionID() << ", ";
                outFile << languageSupport->getMessage("card_number") << ": " << transaction->getCardNumber() << ", ";
                outFile << languageSupport->getMessage("transaction_type") << ": " << transaction->getTransactionType() << ", ";
                outFile << languageSupport->getMessage("amount") << ": " << transaction->getAmount() << "\n";
            }
        }
    } else {
        // Multi-Bank ATM
        auto banks = BankManager::getInstance()->getAllBanks();
        for (const auto& bankPair : banks) {
            auto bank = bankPair.second;
            for (const auto& accountPair : bank->getAllAccounts()) {
                auto account = accountPair.second;
                for (const auto& transaction : account->getTransactionHistory()) {
                    outFile << languageSupport->getMessage("transaction_id") << ": " << transaction->getTransactionID() << ", ";
                    outFile << languageSupport->getMessage("card_number") << ": " << transaction->getCardNumber() << ", ";
                    outFile << languageSupport->getMessage("transaction_type") << ": " << transaction->getTransactionType() << ", ";
                    outFile << languageSupport->getMessage("amount") << ": " << transaction->getAmount() << "\n";
                }
            }
        }
    }

    outFile.close();
    std::cout << languageSupport->getMessage("transaction_history_successfully_exported") << " " << filename << std::endl;
}

// Print cash inventory
void ATM::displayCashInventory() const {
    std::cout << languageSupport->getMessage("atm_serial_number") << ": " << serialNumber << " ";
    cashManager->printCashInventory();
}

// Show main menu
void ATM::showMainMenu() {
    while (true) {
        std::cout << languageSupport->getMessage("select_transaction") << std::endl;
        std::cout << languageSupport->getMessage("main_menu_options") << std::endl;

        int choice = 0;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                handleDeposit();
                break;
            case 2:
                handleWithdrawal();
                break;
            case 3:
                handleTransfer();
                break;
            case 4:
                endSession();
                return;
            default:
                std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
                break;
        }
    }
}

// Handle deposit
void ATM::handleDeposit() {
    // Select deposit type
    std::cout << languageSupport->getMessage("select_deposit_type") << std::endl;
    std::cout << languageSupport->getMessage("deposit_type_options") << std::endl;

    int depositTypeInput;
    while (true) {
        auto depositTypeVariant = InputHandler::getInput("", InputType::INT);
        try {
            depositTypeInput = std::get<int>(depositTypeVariant);
            if (depositTypeInput != 1 && depositTypeInput != 2) {
                std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
                continue;
            }
            break;
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
        }
    }

    DepositType depositType = (depositTypeInput == 1) ? DepositType::CASH : DepositType::CHECK;
    int totalAmount = 0;

    if (depositType == DepositType::CASH) {
        // Accept cash denominations
        std::map<Denomination, int> depositCash;
        int totalBills = 0;

        std::cout << languageSupport->getMessage("denomination_prompt") << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            Denomination denom = denomPair.first;
            int denomValue = denomPair.second;
            int count = 0;
            while (true) {
                auto countVariant = InputHandler::getInput(languageSupport->getMessage("enter_bills") + ": ", InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << languageSupport->getMessage("negative_bill_count") << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                }
            }
            depositCash[denom] = count;
            totalAmount += denomValue * count;
            totalBills += count;
        }

        if (totalAmount == 0) {
            std::cout << languageSupport->getMessage("no_cash_entered") << std::endl;
            return;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << languageSupport->getMessage("max_bills_exceeded") << std::endl;
            return;
        }

        // Create and execute deposit transaction
        auto depositTransaction = TransactionFactory::createDepositTransaction(totalAmount, currentAccount, depositType, currentCardNumber);

        if (depositTransaction->execute()) {
            // Update cash inventory
            try {
                cashManager->acceptCash(depositCash);
            } catch (const ATMException& e) {
                std::cout << e.what() << std::endl;
                depositTransaction->rollback();
                return;
            }

            // Add transaction record
            currentAccount->addTransaction(depositTransaction);
            addSessionTransaction(depositTransaction);
            std::cout << languageSupport->getMessage("deposit_successful") << totalAmount << std::endl;
        } else {
            std::cout << languageSupport->getMessage("deposit_failed") << std::endl;
        }

    } else if (depositType == DepositType::CHECK) {
        // Handle check deposit
        int checkAmount = 0;
        while (true) {
            auto amountVariant = InputHandler::getInput(languageSupport->getMessage("check_deposit_prompt") + "\n", InputType::INT);
            try {
                checkAmount = std::get<int>(amountVariant);
                if (checkAmount <= 0) {
                    std::cout << languageSupport->getMessage("positive_amount_required") << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            }
        }

        if (checkAmount > MAX_CHECK_DEPOSIT) {
            std::cout << languageSupport->getMessage("max_check_deposit_exceeded") << std::endl;
            return;
        }

        // Create and execute deposit transaction
        auto depositTransaction = TransactionFactory::createDepositTransaction(checkAmount, currentAccount, depositType, currentCardNumber);

        if (depositTransaction->execute()) {
            // Add transaction record
            currentAccount->addTransaction(depositTransaction);
            addSessionTransaction(depositTransaction);
            std::cout << languageSupport->getMessage("check_deposit_successful") << checkAmount << std::endl;
        } else {
            std::cout << languageSupport->getMessage("check_deposit_failed") << std::endl;
        }
    }
}

// Modify handleWithdrawal to ensure amounts are multiples of KRW 1,000
void ATM::handleWithdrawal() {
    // Limit of 3 withdrawals per session
    int withdrawalCount = 0;
    for (const auto& txn : sessionTransactions) {
        if (txn->getTransactionType() == "Withdrawal") {
            withdrawalCount++;
        }
    }

    if (withdrawalCount >= 3) {
        std::cout << languageSupport->getMessage("max_withdrawals_exceeded") << std::endl;
        return;
    }

    // Enter withdrawal amount
    int amount = 0;
    while (true) {
        auto amountVariant = InputHandler::getInput(languageSupport->getMessage("withdrawal_amount_prompt") + "\n", InputType::INT);
        try {
            amount = std::get<int>(amountVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (amount <= 0 || amount > MAX_WITHDRAWAL_AMOUNT) {
            std::cout << languageSupport->getMessage("invalid_withdrawal_amount") << std::endl;
            continue;
        }

        if (amount % 1000 != 0) {
            std::cout << languageSupport->getMessage("amount_must_be_multiple") << std::endl;
            continue;
        }

        break;
    }

    // Create transaction
    auto withdrawalTransaction = TransactionFactory::createWithdrawalTransaction(amount, currentAccount, currentCardNumber);

    // Execute transaction
    try {
        if (withdrawalTransaction->execute()) {
            // Dispense cash
            std::map<Denomination, int> dispensedCash;
            if (cashManager->dispenseCash(amount, dispensedCash)) {
                std::cout << languageSupport->getMessage("withdrawal_successful") << std::endl;

                // Add transaction record
                currentAccount->addTransaction(withdrawalTransaction);
                addSessionTransaction(withdrawalTransaction);

                std::cout << languageSupport->getMessage("dispensed_cash") << std::endl;
                for (const auto& pair : dispensedCash) {
                    std::cout << "KRW " << DENOMINATION_VALUES.at(pair.first) << " x " << pair.second << std::endl;
                }
            } else {
                // Cannot dispense the exact amount with available denominations
                withdrawalTransaction->rollback();
                std::cout << languageSupport->getMessage("atm_insufficient_cash") << std::endl;
            }
        }
    } catch (const ATMException& e) {
        std::cout << e.what() << std::endl;
    }
}

// Handle transfer
void ATM::handleTransfer() {
    // Select transfer type
    std::cout << languageSupport->getMessage("select_transfer_type") << std::endl;
    std::cout << languageSupport->getMessage("transfer_type_options") << std::endl;

    int transferTypeInput;
    auto transferTypeVariant = InputHandler::getInput("", InputType::INT);
    try {
        transferTypeInput = std::get<int>(transferTypeVariant);
    } catch (const std::bad_variant_access&) {
        std::cout << languageSupport->getMessage("invalid_input") << std::endl;
        return;
    }

    TransferType transferType;
    if (transferTypeInput == 1) {
        transferType = TransferType::CASH;
    } else if (transferTypeInput == 2) {
        transferType = TransferType::ACCOUNT;
    } else {
        std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
        return;
    }

    // Enter destination account number
    std::string destinationAccountNumber;
    while (true) {
        auto destAccountVariant = InputHandler::getInput(languageSupport->getMessage("enter_destination_account") + "\n", InputType::STRING);
        try {
            destinationAccountNumber = std::get<std::string>(destAccountVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (destinationAccountNumber.empty()) {
            std::cout << languageSupport->getMessage("empty_account_number") << std::endl;
            continue;
        }
        break;
    }

    // Get destination account
    std::shared_ptr<Account> destinationAccount = nullptr;
    if (atmType == ATMType::SINGLE) {
        destinationAccount = primaryBank->getAccount(destinationAccountNumber);
    } else {
        // For Multi-Bank ATM, search all banks
        auto banks = BankManager::getInstance()->getAllBanks();
        for (const auto& bankPair : banks) {
            destinationAccount = bankPair.second->getAccount(destinationAccountNumber);
            if (destinationAccount) {
                break;
            }
        }
    }

    if (!destinationAccount) {
        std::cout << languageSupport->getMessage("destination_account_not_found") << std::endl;
        return;
    }

    // Enter transfer amount
    int amount = 0;
    while (true) {
        auto amountVariant = InputHandler::getInput(languageSupport->getMessage("enter_transfer_amount") + "\n", InputType::INT);
        try {
            amount = std::get<int>(amountVariant);
        } catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (amount <= 0) {
            std::cout << languageSupport->getMessage("positive_amount_required") << std::endl;
            continue;
        }
        break;
    }

    // Create transaction
    auto transferTransaction = TransactionFactory::createTransferTransaction(amount, currentAccount, destinationAccount, transferType, currentCardNumber);

    // Execute transaction
    try {
        if (transferTransaction->execute()) {
            // Add transaction record
            currentAccount->addTransaction(transferTransaction);
            addSessionTransaction(transferTransaction);
            std::cout << languageSupport->getMessage("transfer_successful") << std::endl;
        }
    } catch (const ATMException& e) {
        std::cout << e.what() << std::endl;
    }
}

// End session
void ATM::endSession() {
    printSessionSummary();
    std::cout << languageSupport->getMessage("session_end") << std::endl;
    // Reset session variables
    currentAccount = nullptr;
    currentCardNumber.clear();
    isAdminSession = false;
    sessionTransactions.clear();
}

void ATM::printSessionSummary() {
    if (sessionTransactions.empty()) {
        return;
    }

    std::cout << "\n" << languageSupport->getMessage("session_summary_header") << "\n";
    for (const auto& txn : sessionTransactions) {
        txn->printDetails();
    }
    std::cout << languageSupport->getMessage("end_session_summary") << "\n" << std::endl;
}