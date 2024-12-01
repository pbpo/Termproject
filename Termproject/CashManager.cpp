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
};
CashManager::CashManager() {
    // Initialize with zero cash; the bank will deposit cash during initialization
    for (const auto& pair : DENOMINATION_VALUES) {
        cashInventory[pair.first] = 0;
    }
}

bool CashManager::dispenseCash(int amount, std::map<Denomination, int>& dispensedCash) {
    dispensedCash.clear();
    if (amount < 0) return false;

    // Clone the cashInventory to work with
    std::map<Denomination, int> tempInventory = cashInventory;

    // List of denominations sorted in descending order
    std::vector<Denomination> denominations;
    for (const auto& pair : DENOMINATION_VALUES) {
        denominations.push_back(pair.first);
    }
    std::sort(denominations.begin(), denominations.end(),
              [&](const Denomination& a, const Denomination& b) {
                  return DENOMINATION_VALUES.at(a) > DENOMINATION_VALUES.at(b);
              });

    // 1. 그리디 알고리즘 시도
    int remainingAmount = amount;
    for (const auto& denom : denominations) {
        int denomValue = DENOMINATION_VALUES.at(denom);
        int availableBills = tempInventory[denom];
        if (denomValue > remainingAmount || availableBills == 0) continue;

        int neededBills = remainingAmount / denomValue;
        int usedBills = std::min(neededBills, availableBills);
        if (usedBills > 0) {
            dispensedCash[denom] = usedBills;
            remainingAmount -= denomValue * usedBills;
        }

        if (remainingAmount == 0) break;
    }

    if (remainingAmount == 0) {
        // 그리디 알고리즘으로 정확한 금액을 분배할 수 있었음
        // cashInventory 업데이트
        for (const auto& pair : dispensedCash) {
            cashInventory[pair.first] -= pair.second;
        }
        return true;
    }

    // 2. 그리디 알고리즘 실패 시 BFS 시도
    // Initialize BFS with a regular queue
    std::queue<CashState> q;
    CashState initialState = {0, {}};
    q.push(initialState);

    // To keep track of visited states (amount and bills used)
    std::set<std::pair<int, std::string>> visited; // pair of amount and serialized billsUsed
    auto serialize = [&](const std::map<Denomination, int>& bills) -> std::string {
        std::string s;
        for (const auto& pair : bills) {
            s += std::to_string(pair.second) + ",";
        }
        return s;
    };
    visited.emplace(0, serialize(initialState.billsUsed));

    while (!q.empty()) {
        CashState currentState = q.front();
        q.pop();

        int currentAmount = currentState.currentAmount;

        // Try adding each denomination
        for (const auto& denom : denominations) {
            int denomValue = DENOMINATION_VALUES.at(denom);
            int newAmount = currentAmount + denomValue;

            if (newAmount > amount) continue;

            // Check if there are available bills for this denomination
            int usedBills = currentState.billsUsed[denom];
            if (usedBills >= tempInventory[denom]) continue;

            // Create a new state
            CashState newState = currentState;
            newState.currentAmount = newAmount;
            newState.billsUsed[denom] += 1;

            // Serialize the billsUsed for visited check
            std::string serialized = serialize(newState.billsUsed);
            if (visited.find({newAmount, serialized}) != visited.end()) continue;

            // Check if we've reached the desired amount
            if (newAmount == amount) {
                dispensedCash = newState.billsUsed;

                // Update cash inventory
                for (const auto& pair : dispensedCash) {
                    cashInventory[pair.first] -= pair.second;
                }

                return true;
            }

            // Mark this state as visited and add the new state to the queue
            visited.emplace(newAmount, serialized);
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
