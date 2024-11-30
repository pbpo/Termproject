#include "TransactionLogger.hpp"
#include <chrono>
#include <iomanip>

TransactionLogger::TransactionLogger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("로그 파일을 열 수 없습니다: " + filename);
    }
}

TransactionLogger::~TransactionLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void TransactionLogger::logTransaction(const std::string& transactionID, const std::string& cardNumber, const std::string& transactionType, int amount) {
    std::lock_guard<std::mutex> guard(logMutex);
    if (logFile.is_open()) {
        // 현재 시간 가져오기
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        logFile << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "] "
                << "Transaction ID: " << transactionID
                << ", Card Number: " << cardNumber
                << ", Type: " << transactionType
                << ", Amount: " << amount
                << "\n";
    }
}

// 필요하다면 onTransactionExecuted 함수의 정의 추가
/*
void TransactionLogger::onTransactionExecuted(const std::shared_ptr<ITransaction>& transaction) {
    // 함수 구현 내용
}
*/
