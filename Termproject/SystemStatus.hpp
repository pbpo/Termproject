//
//  SystemStatus.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// SystemStatus.hpp
#pragma once

#include "ATM.hpp"
#include "BankManager.hpp"

class SystemStatus {
private:
    static SystemStatus* instance;
    ATM* atm;

    // Singleton pattern: private constructor
    SystemStatus();

public:
    // Get the singleton instance
    static SystemStatus* getInstance();

    // Set and get ATM
    void setATM(ATM* atm);
    ATM* getATM() const;

    // Keep setBank and getBank methods using Bank*
    void setBank(Bank* bank);
    Bank* getBank() const;

    // Display system status
    void displayStatus();
};