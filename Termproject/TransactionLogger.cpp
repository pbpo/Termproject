// TransactionLogger.cpp
#include "TransactionLogger.hpp"
#include <chrono>
#include <iomanip>
#include <iostream> // 콘솔 출력용 추가
#include <stdexcept>

TransactionLogger::TransactionLogger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("로그 파일을 열 수 없습니다: " + filename);
    }
    std::cout << "TransactionLogger 초기화 완료: " << filename << std::endl;
}

TransactionLogger::~TransactionLogger() {
    if (logFile.is_open()) {
        logFile.close();
        std::cout << "TransactionLogger 소멸자 호출: 로그 파일 닫힘." << std::endl;
    }
}

void TransactionLogger::logTransaction(const std::string& transactionID, const std::string& cardNumber, const std::string& transactionType, int amount) {
    std::lock_guard<std::mutex> guard(logMutex);
    if (logFile.is_open()) {
        try {
            // 디버깅 메시지 추가: 함수 호출 확인
            std::cout << "Logging transaction: " << transactionID 
                      << ", Card Number: " << cardNumber 
                      << ", Type: " << transactionType 
                      << ", Amount: " << amount << std::endl;

            // 현재 시간 가져오기
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            logFile << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "] "
                    << "Transaction ID: " << transactionID
                    << ", Card Number: " << cardNumber
                    << ", Type: " << transactionType
                    << ", Amount: " << amount
                    << "\n";
            logFile.flush(); // 데이터 즉시 기록

            // 디버깅 메시지 추가: 로그 기록 완료 확인
            std::cout << "Transaction logged successfully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "로그 작성 중 예외 발생: " << e.what() << std::endl;
        }
    } else {
        // 로그 파일이 열려 있지 않을 경우 메시지 출력
        std::cerr << "로그 파일이 열려 있지 않습니다." << std::endl;
    }
}
