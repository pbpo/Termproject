//
//  CashManager.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// CashManager.hpp
#pragma once

#include <map>
#include "Utility.hpp"

class CashManager {
private:
    std::map<Denomination, int> cashInventory;
    static CashManager* instance;
    CashManager();

public:
    static CashManager* getInstance();

    bool dispenseCash(int amount, std::map<Denomination, int>& dispensedCash);
    void acceptCash(const std::map<Denomination, int>& cashDetails);
    void addCash(Denomination denom, int count);
    int getTotalCash() const;
    void printCashInventory() const;

    // Additional method
    void withdrawCash(const std::map<Denomination, int>& cashDetails);
};
