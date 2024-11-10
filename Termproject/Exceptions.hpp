//
//  Exceptions.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

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

class InvalidCardException : public ATMException {
public:
    InvalidCardException(const std::string& msg) : ATMException(msg) {}
};

class WrongPasswordException : public ATMException {
public:
    WrongPasswordException(const std::string& msg) : ATMException(msg) {}
};

class InsufficientFundsException : public ATMException {
public:
    InsufficientFundsException() : ATMException("Insufficient funds.") {}
};

class CashLimitExceededException : public ATMException {
public:
    CashLimitExceededException() : ATMException("ATM has insufficient cash.") {}
};

// Add other exception classes as needed
