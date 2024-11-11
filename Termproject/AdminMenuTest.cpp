#include "AdminMenuTest.hpp"
#include <iostream>

using namespace std;

// 생성자 구현
// TestGroup 생성자를 실행할 때 구현한 테스트 개수를 입력해주시기 바랍니다
AdminMenuTest::AdminMenuTest(int groupNumber) : TestGroup(groupNumber, 3) {}

// 테스트 함수 구현
void AdminMenuTest::test_1() {
    cout << "Running AdminMenuTest - Test 1" << endl;
}

void AdminMenuTest::test_2() {
    cout << "Running AdminMenuTest - Test 2" << endl;
}

void AdminMenuTest::test_3() {
    cout << "Running AdminMenuTest - Test 3" << endl;
}
