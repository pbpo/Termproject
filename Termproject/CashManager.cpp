#include "CashManager.hpp"
#include <iostream>
#include <algorithm>
#include "Exceptions.hpp"
#include <queue>
#include <vector>
#include <map>
#include <set>
struct CashState {
    int currentAmount;
    std::map<Denomination, int> billsUsed;

    // For ordering in the queue
    bool operator<(const CashState& other) const {
        return currentAmount > other.currentAmount; // Priority queue uses max heap by default
    }
};
CashManager::CashManager() {
    // Initialize with zero cash; the bank will deposit cash during initialization
    for (const auto& pair : DENOMINATION_VALUES) {
        cashInventory[pair.first] = 0;
    }
}

bool CashManager::dispenseCash(int amount, std::map<Denomination, int>& dispensedCash) {
    dispensedCash.clear();
    if (amount <= 0) return false;

    // Initialize BFS
    std::priority_queue<CashState> q;
    CashState initialState = {0, {}};
    q.push(initialState);

    // To keep track of visited amounts to prevent revisiting
    std::set<int> visited;
    visited.insert(0);

    // List of denominations sorted in descending order
    std::vector<Denomination> denominations;
    for (const auto& pair : DENOMINATION_VALUES) {
        denominations.push_back(pair.first);
    }
    std::sort(denominations.begin(), denominations.end(),
              [&](const Denomination& a, const Denomination& b) {
                  return DENOMINATION_VALUES.at(a) > DENOMINATION_VALUES.at(b);
              });

    while (!q.empty()) {
        CashState currentState = q.top();
        q.pop();

        int currentAmount = currentState.currentAmount;

        // Try adding each denomination
        for (const auto& denom : denominations) {
            int denomValue = DENOMINATION_VALUES.at(denom);
            int newAmount = currentAmount + denomValue;

            if (newAmount > amount) continue;

            // Check if we have already visited this amount
            if (visited.find(newAmount) != visited.end()) continue;

            // Check if there are available bills for this denomination
            int usedBills = currentState.billsUsed[denom];
            if (usedBills >= cashInventory[denom]) continue;

            // Create a new state
            CashState newState = currentState;
            newState.currentAmount = newAmount;
            newState.billsUsed[denom] += 1;

            // Check if we've reached the desired amount
            if (newAmount == amount) {
                dispensedCash = newState.billsUsed;

                // Update cash inventory
                for (const auto& pair : dispensedCash) {
                    cashInventory[pair.first] -= pair.second;
                }

                return true;
            }

            // Mark this amount as visited and add the new state to the queue
            visited.insert(newAmount);
            q.push(newState);
        }
    }

    // If we reach here, dispensing the exact amount is not possible
    return false;
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
