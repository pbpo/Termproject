#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

#include <memory>

// 거래 인터페이스의 전방 선언
class ITransaction;

class IObserver {
public:
    virtual void onTransactionExecuted(const std::shared_ptr<ITransaction>& transaction) = 0;
    virtual ~IObserver() {}
};

#endif // IOBSERVER_HPP