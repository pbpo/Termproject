#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

class TransactionLogger {
private:
    std::ofstream logFile;
    std::mutex logMutex;

public:
    // 생성자 선언
    TransactionLogger(const std::string& filename);

    // 소멸자 선언
    ~TransactionLogger();

    // 거래 정보를 로그에 기록
    void logTransaction(const std::string& transactionID, const std::string& cardNumber, const std::string& transactionType, int amount);

    // 필요하다면 onTransactionExecuted 함수 선언 추가
    // void onTransactionExecuted(const std::shared_ptr<ITransaction>& transaction);
};
