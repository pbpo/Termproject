#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "IObserver.hpp"
#include <vector>
#include <memory>
#include <algorithm>

class ISubject {
public:
    virtual void attach(const std::shared_ptr<IObserver>& observer) = 0;
    virtual void detach(const std::shared_ptr<IObserver>& observer) = 0;
    virtual void notify(const std::shared_ptr<ITransaction>& transaction) = 0;
    virtual ~ISubject() {}
};

#endif // ISUBJECT_HPP