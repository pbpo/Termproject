#include "TestGroup.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

TestGroup::TestGroup(int group, int count)
    : groupNumber(group), testCount(count) {}

TestGroup::~TestGroup() {}

string TestGroup::formatTestId(int testIndex) const {
    return to_string(groupNumber) + "." + to_string(testIndex);
}

void TestGroup::execute() {
    // 전체 테스트 실행
    for (int i = 1; i <= testCount; ++i) {
        cout << "Running test for REQ " << formatTestId(i) << "...\n";
        callTest(i);
    }
}

void TestGroup::execute(int testIndex) {
    try {
        if (testIndex > testCount || testIndex < 1) {
            throw invalid_argument("Invalid test ID: " + formatTestId(testIndex));
        }
        cout << "Running test for REQ" << formatTestId(testIndex) << "...\n";
        callTest(testIndex); // 특정 테스트 실행
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        throw;
    }
}

void TestGroup::callTest(int testIndex) {
    switch (testIndex) {
    case 1: test_1(); break;
    case 2: test_2(); break;
    case 3: test_3(); break;
    case 4: test_4(); break;
    case 5: test_5(); break;
    case 6: test_6(); break;
    case 7: test_7(); break;
    case 8: test_8(); break;
    case 9: test_9(); break;
    case 10: test_10(); break;
    case 11: test_11(); break;
    default:
        throw invalid_argument("Invalid test ID: " + formatTestId(testIndex));
    }
}

void TestGroup::notImplemented(int testIndex) {
    throw invalid_argument("Not implemented: " + formatTestId(testIndex));
}

// 기본 테스트 함수 구현
void TestGroup::test_1() { notImplemented(1); }
void TestGroup::test_2() { notImplemented(2); }
void TestGroup::test_3() { notImplemented(3); }
void TestGroup::test_4() { notImplemented(4); }
void TestGroup::test_5() { notImplemented(5); }
void TestGroup::test_6() { notImplemented(6); }
void TestGroup::test_7() { notImplemented(7); }
void TestGroup::test_8() { notImplemented(8); }
void TestGroup::test_9() { notImplemented(9); }
void TestGroup::test_10() { notImplemented(10); }
void TestGroup::test_11() { notImplemented(11); }
