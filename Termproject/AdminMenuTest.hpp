#ifndef ADMIN_MENU_TEST_HPP
#define ADMIN_MENU_TEST_HPP

#include "TestGroup.hpp"

class AdminMenuTest : public TestGroup {
private:
    //여기에 테스트에 필요한 데이터 선언하시면 됩니다
public:
    AdminMenuTest(int groupNumber);
    void test_1() override;
    void test_2() override;
    void test_3() override;
};
#endif