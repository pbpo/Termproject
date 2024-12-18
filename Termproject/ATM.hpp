//
//  ATM.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <set>
// Include necessary headers
#include "Utility.hpp"
#include "Bank.hpp"
#include "CashManager.hpp"
#include "SecurityManager.hpp"
#include "LanguageSupport.hpp"
#include "ITransaction.hpp"
#include "InputHandler.hpp"
#include "Account.hpp"
#include "TransactionLogger.hpp"
#include "BankManager.hpp"

class ATM {
private:
    // ATM unique information
    static std::set<std::string> assignedSerialNumbers;
    std::string serialNumber;

    ATMType atmType;
    BankManager* bankManager;
    std::shared_ptr<Bank> primaryBank;
    bool isBilingual;

    // Admin and system management tools
    LanguageSupport* languageSupport;
    CashManager* cashManager;
    SecurityManager* securityManager;

    // Current session information
    std::string currentCardNumber;
    std::shared_ptr<Account> currentAccount;
    std::vector<std::shared_ptr<ITransaction>> sessionTransactions;
    int wrongPasswordAttempts;
    int  checkDeposits;
    int cashDeposits;
    bool isAdminSession;
    std::unique_ptr<TransactionLogger> transactionLogger;
    std::vector<std::string> transactionHistory; // 거래 내역 저장

    // Internal methods
    void showMainMenu();
    void handleDeposit();
    void handleWithdrawal();
    void handleTransfer();
    void handleAdminMenu();
    void displayAllTransactionHistory() const;
    void exportTransactionHistoryToFile(const std::string& filename) const;
    void endSession();
    void printSessionSummary();

public:
    // Constructor
    ATM(const std::string& serialNumber, ATMType atmType, CashManager* cashManager, std::shared_ptr<Bank> primaryBank, bool isBilingual);
    ATM(const std::string& serialNumber, ATMType atmType, CashManager* cashManager, bool isBilingual);
    // Session management
    void startSession();

    // Display cash inventory
    void displayCashInventory() const;
    ATMType getATMType() const { return atmType; }
    bool getIsBilingual() const { return isBilingual; }
    // Transaction record management
    void addSessionTransaction(const std::shared_ptr<ITransaction>& transaction);
    std::string& getSerialNumber() { return serialNumber; }


};
