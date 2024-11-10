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
                cashInventory[denom] -= dispensedCount;
            }
        }
    }

    if (remainingAmount == 0) {
        return true;
    } else {
        // Rollback
        for (const auto& pair : dispensedCash) {
            cashInventory[pair.first] += pair.second;
        }
        dispensedCash.clear();
        throw CashLimitExceededException();
    }
}

void CashManager::acceptCash(const std::map<Denomination, int>& cashDetails) {
    for (const auto& pair : cashDetails) {
        cashInventory[pair.first] += pair.second;
    }
}

void CashManager::addCash(Denomination denom, int count) {
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
        if (cashInventory.find(pair.first) != cashInventory.end()) {
            cashInventory[pair.first] -= pair.second;
        }
    }
}
