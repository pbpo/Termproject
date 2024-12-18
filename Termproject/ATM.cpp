#include "ATM.hpp"
#include "DepositTransaction.hpp"
#include "WithdrawalTransaction.hpp"
#include "TransferTransaction.hpp"
#include "Exceptions.hpp"
#include "InputHandler.hpp"
#include "SystemStatus.hpp"
#include "TransactionFactory.hpp"
#include <iostream>
#include <variant>
#include <fstream>
#include "SecurityManager.hpp"
#include "BankManager.hpp"
#include "TransactionLogger.hpp"
#include <filesystem>

std::set<std::string> ATM::assignedSerialNumbers;

// Constructor for Single-Bank ATM
ATM::ATM(const std::string& serialNumber, ATMType atmType, CashManager* cashManager, std::shared_ptr<Bank> primaryBank, bool isBilingual)
    : atmType(atmType),
    bankManager(BankManager::getInstance()),
    primaryBank(primaryBank),
    languageSupport(LanguageSupport::getInstance()),
    cashManager(cashManager),
    securityManager(SecurityManager::getInstance()),
    wrongPasswordAttempts(0),
     checkDeposits(0),
    cashDeposits(0),
    isAdminSession(false),
    transactionLogger(nullptr) {

    // Serial number validation
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
        auto choiceVariant = InputHandler::getInput(languageSupport->getMessage("select_language") + "\n", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            choice = 1;
        }

        if (choice == 1) {
            languageSupport->setLanguage(Language::ENGLISH);
        }
        else if (choice == 2) {
            languageSupport->setLanguage(Language::KOREAN);
        }
        else {
            languageSupport->setLanguage(Language::ENGLISH);
        }
    }

    // Initialize TransactionLogger
    try {
        std::string logPath = "transaction_log.txt";

        std::filesystem::path pathObj(logPath);
        if (!pathObj.parent_path().empty() && !std::filesystem::exists(pathObj.parent_path())) {
            std::filesystem::create_directories(pathObj.parent_path());
            std::cout << "Created directories: " << pathObj.parent_path() << std::endl;
        }

        std::cout << "Initializing TransactionLogger with path: " << logPath << std::endl;
        transactionLogger = std::make_unique<TransactionLogger>(logPath);
    }
    catch (const std::exception& e) {
        std::cerr << "TransactionLogger 초기화 실패: " << e.what() << std::endl;
    }

}

// Constructor for Multi-Bank ATM
ATM::ATM(const std::string& serialNumber,  ATMType atmType, CashManager* cashManager, bool isBilingual)
    : atmType(atmType),
    bankManager(BankManager::getInstance()),
    primaryBank(nullptr),
    languageSupport(LanguageSupport::getInstance()),
    cashManager(cashManager),
    securityManager(SecurityManager::getInstance()),
    wrongPasswordAttempts(0),
    isAdminSession(false),
    transactionLogger(nullptr) {

    // Serial number validation
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

    // Language support setup
    this->isBilingual = isBilingual;
    if (isBilingual) {
        int choice = 0;
        std::cout << languageSupport->getMessage("select_language") << std::endl;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input_default_language") << std::endl;
            choice = 1;
        }

        if (choice == 1) {
            languageSupport->setLanguage(Language::ENGLISH);
        }
        else if (choice == 2) {
            languageSupport->setLanguage(Language::KOREAN);
        }
        else {
            languageSupport->setLanguage(Language::ENGLISH);
        }
    }

    // Initialize TransactionLogger
    try {
        std::string logPath = "transaction_log.txt";

        std::filesystem::path pathObj(logPath);
        if (!pathObj.parent_path().empty() && !std::filesystem::exists(pathObj.parent_path())) {
            std::filesystem::create_directories(pathObj.parent_path());
            std::cout << "Created directories: " << pathObj.parent_path() << std::endl;
        }

        std::cout << "Initializing TransactionLogger with path: " << logPath << std::endl;
        transactionLogger = std::make_unique<TransactionLogger>(logPath);
    }
    catch (const std::exception& e) {
        std::cerr << "TransactionLogger 초기화 실패: " << e.what() << std::endl;
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
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        try {
            bool isValidCard = false;
            if (atmType == ATMType::SINGLE) {
                isValidCard = securityManager->validateCard(cardNumber, primaryBank->getBankName());
            }
            else {
                // For Multi-Bank ATM, any bank's card is valid
                
                isValidCard = securityManager->validateCard(cardNumber);
            }

            // Admin card check
            if (cardNumber == "000000") {
                isAdminSession = true;
                handleAdminMenu();
                endSession();
                continue;
            }

            if (!isValidCard) {
                throw InvalidCardException(languageSupport->getMessage("card_invalid"));
            }



            // User authentication
            while (wrongPasswordAttempts < 3) {
                auto passwordVariant = InputHandler::getInput(languageSupport->getMessage("enter_password") + "\n", InputType::STRING);
                std::string password;
                try {
                    password = std::get<std::string>(passwordVariant);
                }
                catch (const std::bad_variant_access&) {
                    std::cout << languageSupport->getMessage("invalid_input") << std::endl;
                    wrongPasswordAttempts++;
                    continue;
                }

                if (securityManager->authenticateUser(cardNumber, password)) {
                    // Authentication successful
                    currentAccount = bankManager->getAccount(cardNumber);
                    if (!currentAccount) {
                        std::cout << languageSupport->getMessage("Account not found. Please contact the bank.") << std::endl;
                        break;
                    }
                    currentCardNumber = cardNumber;
                    showMainMenu();
                    break;
                }
                else {
                    wrongPasswordAttempts++;
                    std::cout << languageSupport->getMessage("wrong_password") << std::endl;
                }
            }

            if (wrongPasswordAttempts >= 3) {
                throw WrongPasswordException(languageSupport->getMessage("max_password_attempts"));
            }
        }
        catch (const ATMException& e) {
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
        std::cout << "Admin Menu:" << std::endl;
        std::cout << "1. View Transaction History\n2. Export Transaction History to File\n3. Exit Admin Menu" << std::endl;

        int choice = 0;
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        switch (choice) {
        case 1:
            displayAllTransactionHistory();
            break;
        case 2:
            exportTransactionHistoryToFile("transaction_history.txt");
            std::cout << "Transaction history exported to transaction_history.txt" << std::endl;
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
    std::cout << "\n--- All Transaction History ---" << std::endl;
    for (const auto& pair : bankManager->getAllAccounts()) {
        auto account = pair.second;
        for (const auto& transaction : account->getTransactionHistory()) {
            transaction->printDetails();
        }
    }
    std::cout << "--- End of Transaction History ---\n" << std::endl;
}

// Export transaction history to file
void ATM::exportTransactionHistoryToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Failed to open file for writing." << std::endl;
        return;
    }

    outFile << "=== Transaction History ===\n";
    for (const auto& pair : bankManager->getAllAccounts()) {
        auto account = pair.second;
        for (const auto& transaction : account->getTransactionHistory()) {
            outFile << "Transaction ID: " << transaction->getTransactionID() << ", ";
            outFile << "Card Number: " << transaction->getCardNumber() << ", ";
            outFile << "Type: " << transaction->getTransactionType() << ", ";
            outFile << "Amount: " << transaction->getAmount() << "\n";
        }
    }
    outFile.close();
    std::cout << "Transaction history successfully exported to " << filename << std::endl;
}

// Print cash inventory
void ATM::displayCashInventory() const {
    std::cout << "ATM [SN: " << serialNumber << "] ";
    cashManager->printCashInventory();
}

// Show main menu
void ATM::showMainMenu() {
    while (true) {
        std::cout << languageSupport->getMessage("select_transaction") << std::endl;
        std::cout << languageSupport->getMessage("1. Deposit\n2. Withdrawal\n3. Transfer\n4. Exit") << std::endl;

        int choice = 0;
        
        auto choiceVariant = InputHandler::getInput("", InputType::INT);
        try {
            choice = std::get<int>(choiceVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }
            if(checkDeposits>=30||cashDeposits>=50){
                    checkDeposits=0;
        cashDeposits=0;
         wrongPasswordAttempts = 0;
            std::cout << languageSupport->getMessage("exceed_limit") << std::endl;
            endSession();
            return;
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
        wrongPasswordAttempts = 0;
        checkDeposits=0;
        cashDeposits=0;
            endSession();
            return;
        default:
            std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
            break;
        }
    }
}

void ATM::handleDeposit() {
            if(checkDeposits>=30||cashDeposits>=50){
                    checkDeposits=0;
        cashDeposits=0;
         wrongPasswordAttempts = 0;
            std::cout << languageSupport->getMessage("exceed_limit") << std::endl;
            endSession();
            return;
        }
    // 입금 유형 선택
    std::cout << languageSupport->getMessage("select_deposit_type") << std::endl;
    std::cout << "1. " << languageSupport->getMessage("cash") << " 2. " << languageSupport->getMessage("check") << std::endl;
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
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
        }
    }

    DepositType depositType = (depositTypeInput == 1) ? DepositType::CASH : DepositType::CHECK;

    // 입금 금액 입력
    int amount = 0;
    while (true) {

        try {

            if (depositType == DepositType::CHECK) {
                auto amountVariant = InputHandler::getInput(languageSupport->getMessage("enter_deposit_amount"), InputType::INT);
               
                amount = std::get<int>(amountVariant);
                if (amount <= 0) {
                    std::cout << languageSupport->getMessage("invalid_amount") << std::endl;
                    continue;
                }
                if (amount < 100000) {

                    std::cout << languageSupport->getMessage("min_check_amount") << std::endl;
                    continue;
                }
            }
            break;
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
        }
        
        auto depositTransaction = TransactionFactory::createDepositTransaction(primaryBank->getBankName(),cashManager, amount, currentAccount, depositType, currentCardNumber);

        // 트랜잭션 실행
     if (depositTransaction->execute()) {

        // 거래 기록 추가
        currentAccount->addTransaction(depositTransaction);
        addSessionTransaction(depositTransaction);
        std::cout << languageSupport->getMessage("deposit_successful") << std::endl;

        // 로그 기록
        if (transactionLogger) {
            transactionLogger->logTransaction(
                depositTransaction->getTransactionID(),
                depositTransaction->getCardNumber(),
                depositTransaction->getTransactionType(),
                depositTransaction->getAmount()
            );
        }

        // 입금 횟수 증가

    }
    else {
        std::cout << languageSupport->getMessage("deposit_failed") << std::endl;
    }
}
    // DepositTransaction 객체 생성
    auto depositTransaction = TransactionFactory::createDepositTransaction(primaryBank->getBankName(),cashManager, amount, currentAccount, depositType, currentCardNumber);
                std::shared_ptr<DepositTransaction> specificDepositTransaction = std::dynamic_pointer_cast<DepositTransaction>(depositTransaction);
    if (depositType == DepositType::CHECK) {
            checkDeposits += 1;
        } 
    // 트랜잭션 실행
    if (depositTransaction->execute()) {
        // 거래 기록 추가
        currentAccount->addTransaction(depositTransaction);
        addSessionTransaction(depositTransaction);
        std::cout << languageSupport->getMessage("deposit_successful") << std::endl;
        if(depositType == DepositType::CASH){
            
            cashDeposits += specificDepositTransaction->total();
        }
        if (transactionLogger) {
            transactionLogger->logTransaction(
                depositTransaction->getTransactionID(),
                depositTransaction->getCardNumber(),
                depositTransaction->getTransactionType(),
                depositTransaction->getAmount()
            );
        }
    }
    else {
        std::cout << languageSupport->getMessage("deposit_failed") << std::endl;
    }
}


// Handle Withdrawal
void ATM::handleWithdrawal() {
    // Limit of 3 withdrawals per session
    int withdrawalCount = 0;
    for (const auto& txn : sessionTransactions) {
        if (txn->getTransactionType() == "Withdrawal") {
            withdrawalCount++;
        }
    }

    if (withdrawalCount >= 3) {
        std::cout << languageSupport->getMessage("Maximum number of withdrawals per session exceeded.") << std::endl;
        return;
    }

    // Enter withdrawal amount
    int amount = 0;
    while (true) {
        auto amountVariant = InputHandler::getInput("Enter amount to withdraw:\n", InputType::INT);
        try {
            amount = std::get<int>(amountVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (amount <= 0 || amount > MAX_WITHDRAWAL_AMOUNT) {
            std::cout << languageSupport->getMessage("Amount must be positive and less than or equal to KRW 500,000. Please try again.") << std::endl;
            continue;
        }
        break;
    }

    // Create transaction
    auto withdrawalTransaction = TransactionFactory::createWithdrawalTransaction(primaryBank->getBankName(),cashManager, amount, currentAccount, currentCardNumber);

    // Execute transaction
    try {
        if (withdrawalTransaction->execute()) {
            // Dispense cash
            
            if (true) {
                std::cout <<languageSupport->getMessage("Withdrawal successful.") << std::endl;

                // Add transaction record
                currentAccount->addTransaction(withdrawalTransaction);
                addSessionTransaction(withdrawalTransaction);

                // 로그 기록
                if (transactionLogger) {
                    transactionLogger->logTransaction(
                        withdrawalTransaction->getTransactionID(),
                        withdrawalTransaction->getCardNumber(),
                        withdrawalTransaction->getTransactionType(),
                        withdrawalTransaction->getAmount()
                    );
                }

                std::cout << languageSupport->getMessage("Dispensed cash:")<< std::endl;
                
            }
            else {
                // Insufficient cash in ATM
                withdrawalTransaction->rollback();
                std::cout << languageSupport->getMessage("ATM has insufficient cash. Transaction rolled back.") << std::endl;
            }
        }
    }
    catch (const ATMException& e) {
        std::cout << e.what() << std::endl;
    }
}

// Handle Transfer
void ATM::handleTransfer() {
    // Select transfer type
    std::cout << languageSupport->getMessage("Select transfer type: 1. Cash Transfer 2. Account Transfer") << std::endl;
    int transferTypeInput;
    auto transferTypeVariant = InputHandler::getInput("", InputType::INT);
    try {
        transferTypeInput = std::get<int>(transferTypeVariant);
    }
    catch (const std::bad_variant_access&) {
        std::cout << languageSupport->getMessage("invalid_input") << std::endl;
        return;
    }

    TransferType transferType;
    if (transferTypeInput == 1) {
        transferType = TransferType::CASH;
    }
    else if (transferTypeInput == 2) {
        transferType = TransferType::ACCOUNT;
    }
    else {
        std::cout << languageSupport->getMessage("invalid_choice") << std::endl;
        return;
    }

    // Enter destination account number
    std::string destinationAccountNumber;
    while (true) {
        auto destAccountVariant = InputHandler::getInput("Enter destination account number:\n", InputType::STRING);
        try {
            destinationAccountNumber = std::get<std::string>(destAccountVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (destinationAccountNumber.empty()) {
            std::cout << languageSupport->getMessage("Account number cannot be empty. Please try again.") << std::endl;
            continue;
        }
        break;
    }

    // Get destination account
    auto destinationAccount = bankManager->getAccount(destinationAccountNumber);
    if (!destinationAccount) {
        std::cout << languageSupport->getMessage("Destination account not found.") << std::endl;
        return;
    }
    
    // Enter transfer amount
    int amount = 0;
    if(transferTypeInput == 2){
    while (true) {
        auto amountVariant = InputHandler::getInput("Enter amount to transfer:\n", InputType::INT);
        try {
            amount = std::get<int>(amountVariant);
        }
        catch (const std::bad_variant_access&) {
            std::cout << languageSupport->getMessage("invalid_input") << std::endl;
            continue;
        }

        if (amount <= 0) {
            std::cout << languageSupport->getMessage("Amount must be positive. Please try again.") << std::endl;
            continue;
        }
        break;
    }}
    
 
    // Create transaction
    auto transferTransaction = TransactionFactory::createTransferTransaction(primaryBank->getBankName(),cashManager,amount, currentAccount, destinationAccount, transferType, currentCardNumber);

    // Execute transaction
    try {
        if (transferTransaction->execute()) {
            // Add transaction record
            currentAccount->addTransaction(transferTransaction);

            // 로그 기록
            if (transactionLogger) {
                transactionLogger->logTransaction(
                    transferTransaction->getTransactionID(),
                    transferTransaction->getCardNumber(),
                    transferTransaction->getTransactionType(),
                    transferTransaction->getAmount()
                );
            }
            std::cout << languageSupport->getMessage("Transfer successful.") << std::endl;
        }
    }
    catch (const ATMException& e) {
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

// Print session summary
void ATM::printSessionSummary() {
    if (sessionTransactions.empty()) {
        return;
    }

    std::cout << "\n--- Session Summary ---\n";
    for (const auto& txn : sessionTransactions) {
        txn->printDetails();
    }
    std::cout << "--- End of Summary ---\n" << std::endl;
}
