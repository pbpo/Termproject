//
//  Exceptions.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// Exceptions.hpp
// Exceptions.hpp
#pragma once

#include <exception>
#include <string>

class ATMException : public std::exception {
protected:
    std::string message;

public:
    explicit ATMException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

// New exception classes for serial number validation
class InvalidSerialNumberException : public ATMException {
public:
    explicit InvalidSerialNumberException(const std::string& msg) : ATMException(msg) {}
};

class DuplicateSerialNumberException : public ATMException {
public:
    explicit DuplicateSerialNumberException(const std::string& msg) : ATMException(msg) {}
};

// Existing exception classes
class InvalidCardException : public ATMException {
public:
    explicit InvalidCardException(const std::string& msg) : ATMException(msg) {}
};

class WrongPasswordException : public ATMException {
public:
    explicit WrongPasswordException(const std::string& msg) : ATMException(msg) {}
};

class InsufficientFundsException : public ATMException {
public:
    explicit InsufficientFundsException(const std::string& msg = "Insufficient funds.") : ATMException(msg) {}
};

class CashLimitExceededException : public ATMException {
public:
    explicit CashLimitExceededException(const std::string& msg = "ATM has insufficient cash.") : ATMException(msg) {}
};

class InvalidDenominationException : public ATMException {
public:
    explicit InvalidDenominationException(const std::string& msg) : ATMException(msg) {}
};

// Invalid Amount Exception
class InvalidAmountException : public ATMException {
public:
    explicit InvalidAmountException(const std::string& msg) : ATMException(msg) {}
};
