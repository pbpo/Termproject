#ifndef LANGUAGE_SUPPORT_TEST_HPP
#define LANGUAGE_SUPPORT_TEST_HPP

#include "TestGroup.hpp"

class LanguageSupportTest : public TestGroup {
private:
    //���⿡ �׽�Ʈ�� �ʿ��� ������ �����Ͻø� �˴ϴ�
public:
    LanguageSupportTest(int groupNumber);
    void test_1() override;
    void test_2() override;
    void initializeSystemWithTestData();
};
#endif