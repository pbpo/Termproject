#pragma once

#include <string>
#include <map>

enum class ATMType { SINGLE, MULTI };
enum class TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, ADMIN };
enum class Language { ENGLISH, KOREAN };

enum class TransferType { CASH, ACCOUNT };
enum class DepositType { CASH, CHECK };

enum class Denomination {
    KRW_1000,
    KRW_5000,
    KRW_10000,
    KRW_50000
};

const std::map<Denomination, int> DENOMINATION_VALUES = {
    {Denomination::KRW_1000, 1000},
    {Denomination::KRW_5000, 5000},
    {Denomination::KRW_10000, 10000},
    {Denomination::KRW_50000, 50000}
};

const int MAX_CASH_DEPOSIT = 50;
const int MAX_CHECK_DEPOSIT = 30;
const int MAX_WITHDRAWAL_AMOUNT = 500000;

