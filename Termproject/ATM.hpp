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

// Include necessary headers
#include "Utility.hpp"
#include "Bank.hpp"
#include "CashManager.hpp"
#include "SecurityManager.hpp"
#include "LanguageSupport.hpp"
#include "ITransaction.hpp"
#include "InputHandler.hpp"
#include "Account.hpp"

class ATM {
private:
    // ATM unique information
    std::string serialNumber;
    ATMType atmType;
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
    bool isAdminSession;

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
    ATM(const std::string& serialNumber, ATMType atmType, std::shared_ptr<Bank> primaryBank, bool isBilingual);

    // Session management
    void startSession();

    // Display cash inventory
    void displayCashInventory() const;

    // Transaction record management
    void addSessionTransaction(const std::shared_ptr<ITransaction>& transaction);
};
