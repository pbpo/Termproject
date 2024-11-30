#pragma once

#include <map>
#include "Utility.hpp"

class CashManager {
private:
    std::map<Denomination, int> cashInventory;

public:
    // Constructor
    CashManager();

    // Destructor
    ~CashManager() = default;

    // Methods to manage cash
    bool dispenseCash(int amount, std::map<Denomination, int>& dispensedCash);
    void acceptCash(const std::map<Denomination, int>& cashDetails);
    void addCash(Denomination denom, int count);
    int getTotalCash() const;
    void printCashInventory() const;

    // Additional method
    void withdrawCash(const std::map<Denomination, int>& cashDetails);
};
