#ifndef ADMIN_MENU_TEST_HPP
#define ADMIN_MENU_TEST_HPP

#include "TestGroup.hpp"

class AdminMenuTest : public TestGroup {
private:
    //���⿡ �׽�Ʈ�� �ʿ��� ������ �����Ͻø� �˴ϴ�
public:
    AdminMenuTest(int groupNumber);
    void test_1() override;
    void test_2() override;
    void test_3() override;
};
#endif