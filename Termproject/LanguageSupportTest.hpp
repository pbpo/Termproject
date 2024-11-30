#ifndef LANGUAGE_SUPPORT_TEST_HPP
#define LANGUAGE_SUPPORT_TEST_HPP

#include "TestGroup.hpp"

class LanguageSupportTest : public TestGroup {
private:
    //여기에 테스트에 필요한 데이터 선언하시면 됩니다
public:
    LanguageSupportTest(int groupNumber);
    void test_1() override;
    void test_2() override;
    void initializeSystemWithTestData();
};
#endif