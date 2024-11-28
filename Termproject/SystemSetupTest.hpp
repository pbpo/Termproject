#ifndef SYSTEM_SETUP_TEST_HPP
#define SYSTEM_SETUP_TEST_HPP

#include "TestGroup.hpp"

class SystemSetupTest : public TestGroup {
private:
    // 필요한 데이터 멤버 추가 가능
public:
    SystemSetupTest(int groupNumber);

    // 테스트 함수 구현
    void test_1() override;  // REQ1.1
    void test_2() override;  // REQ1.2
    void test_3() override;  // REQ1.3
    void test_4() override;  // REQ1.4
    void test_5() override;  // REQ1.5
    void test_6() override;  // REQ1.6
    void test_7() override;  // REQ1.7
    void test_8() override;  // REQ1.8
    void test_9() override;  // REQ1.9
    void test_10() override; // REQ1.10
    void test_11() override; // REQ1.11
};

#endif // SYSTEM_SETUP_TEST_HPP
