#include "AdminMenuTest.hpp"
#include <iostream>

using namespace std;

// ������ ����
// TestGroup �����ڸ� ������ �� ������ �׽�Ʈ ������ �Է����ֽñ� �ٶ��ϴ�
AdminMenuTest::AdminMenuTest(int groupNumber) : TestGroup(groupNumber, 3) {}

// �׽�Ʈ �Լ� ����
void AdminMenuTest::test_1() {
    cout << "Running AdminMenuTest - Test 1" << endl;
}

void AdminMenuTest::test_2() {
    cout << "Running AdminMenuTest - Test 2" << endl;
}

void AdminMenuTest::test_3() {
    cout << "Running AdminMenuTest - Test 3" << endl;
}
