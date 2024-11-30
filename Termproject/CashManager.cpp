// CashManager.cpp
#include "CashManager.hpp"
#include <iostream>
#include <algorithm>
#include "Exceptions.hpp"

CashManager* CashManager::instance = nullptr;

CashManager::CashManager() {
    // Initialize with zero cash; the bank will deposit cash during initialization
    for (const auto& pair : DENOMINATION_VALUES) {
        cashInventory[pair.first] = 0;
    }
}

CashManager* CashManager::getInstance() {
    if (instance == nullptr) {
        instance = new CashManager();
    }
    return instance;
}

bool CashManager::dispenseCash(int amount, std::map<Denomination, int>& dispensedCash) {
    dispensedCash.clear();
    int remainingAmount = amount;

    // Start from highest denomination
    for (auto it = DENOMINATION_VALUES.rbegin(); it != DENOMINATION_VALUES.rend(); ++it) {
        Denomination denom = it->first;
        int denomValue = it->second;
        int availableCount = cashInventory[denom];
        int neededCount = remainingAmount / denomValue;

        if (neededCount > 0) {
            int dispensedCount = std::min(neededCount, availableCount);
            if (dispensedCount > 0) {
                dispensedCash[denom] = dispensedCount;
                remainingAmount -= dispensedCount * denomValue;
                // Do not update cashInventory here; update after confirming the transaction
            }
        }
    }

    if (remainingAmount == 0) {
        // Update the cash inventory after successfully calculating the dispensed cash
        for (const auto& pair : dispensedCash) {
            cashInventory[pair.first] -= pair.second;
        }
        return true;
    } else {
        // Cannot dispense the exact amount with available denominations
        dispensedCash.clear();
        return false;
    }
}

void CashManager::acceptCash(const std::map<Denomination, int>& cashDetails) {
    int totalBills = 0;
    for (const auto& pair : cashDetails) {
        Denomination denom = pair.first;
        int count = pair.second;

        if (DENOMINATION_VALUES.find(denom) == DENOMINATION_VALUES.end()) {
            throw InvalidDenominationException("Invalid denomination entered.");
        }

        if (count < 0) {
            throw InvalidAmountException("Count cannot be negative.");
        }

        totalBills += count;

        // Update cash inventory
        cashInventory[denom] += count;
    }

    if (totalBills > MAX_CASH_DEPOSIT) {
        // Rollback the added cash
        for (const auto& pair : cashDetails) {
            cashInventory[pair.first] -= pair.second;
        }
        throw CashLimitExceededException("Maximum cash deposit limit exceeded.");
    }
}

void CashManager::addCash(Denomination denom, int count) {
    if (DENOMINATION_VALUES.find(denom) == DENOMINATION_VALUES.end()) {
        throw InvalidDenominationException("Invalid denomination entered.");
    }

    if (count < 0) {
        throw InvalidAmountException("Count cannot be negative.");
    }

    cashInventory[denom] += count;
}

int CashManager::getTotalCash() const {
    int total = 0;
    for (const auto& pair : cashInventory) {
        total += DENOMINATION_VALUES.at(pair.first) * pair.second;
    }
    return total;
}

void CashManager::printCashInventory() const {
    std::cout << "ATM remaining cash: { ";
    for (const auto& pair : cashInventory) {
        std::cout << "KRW " << DENOMINATION_VALUES.at(pair.first) << " : " << pair.second << ", ";
    }
    std::cout << "}" << std::endl;
}

void CashManager::withdrawCash(const std::map<Denomination, int>& cashDetails) {
    for (const auto& pair : cashDetails) {
        Denomination denom = pair.first;
        int count = pair.second;

        if (DENOMINATION_VALUES.find(denom) == DENOMINATION_VALUES.end()) {
            throw InvalidDenominationException("Invalid denomination entered.");
        }

        if (count < 0) {
            throw InvalidAmountException("Count cannot be negative.");
        }

        if (cashInventory[denom] < count) {
            throw InsufficientFundsException("Insufficient cash in ATM.");
        }

        cashInventory[denom] -= count;
    }
}
