//
//  SystemStatus.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// SystemStatus.hpp
#pragma once

class ATM;
class Bank;

class SystemStatus {
private:
    static SystemStatus* instance;
    ATM* atm;
    Bank* bank;

    SystemStatus();
public:
    static SystemStatus* getInstance();
    void setATM(ATM* atm);
    void setBank(Bank* bank);
    void displayStatus();

    Bank* getBank() const { return bank; }
};
