#ifndef TEST_GROUP_HPP
#define TEST_GROUP_HPP

#include <string>

class TestGroup {
protected:
    int testCount;
    int groupNumber;

public:
    TestGroup(int group, int count);
    virtual ~TestGroup();

    std::string formatTestId(int testIndex) const;
    void execute();
    void execute(int testIndex);

protected:
    void callTest(int testIndex);
    void notImplemented(int testIndex);

    // 테스트 함수 - 자식에서 오버라이드 필요
    virtual void test_1();
    virtual void test_2();
    virtual void test_3();
    virtual void test_4();
    virtual void test_5();
    virtual void test_6();
    virtual void test_7();
    virtual void test_8();
    virtual void test_9();
    virtual void test_10();
    virtual void test_11();
};

#endif // TEST_GROUP_HPP