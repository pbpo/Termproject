#include <iostream>
#include <unordered_map>
#include <memory>

#include "TestGroup.hpp"
// 기능별 test 클래스 import
#include "AdminMenuTest.hpp"

using namespace std;

void runTest() {
    unordered_map<int, shared_ptr<TestGroup>> testGroups;

    // 테스트 그룹 추가 (예시)
    //testGroups[1] = make_shared<DepositTest>(1); // groupNumber = 1
    //testGroups[2] = make_shared<ATMSessionTest>(2);
    //testGroups[3] = make_shared<UserAuthorizationTest>(3); 
    //testGroups[4] = make_shared<DepositTest>(4); 
    //testGroups[5] = make_shared<WithdrawalTest>(5);
    //testGroups[6] = make_shared<TransferTest>(6);
    testGroups[7] = make_shared<AdminMenuTest>(7);
    //testGroups[8] = make_shared<LanguageSupporTest>(8);
    //testGroups[9] = make_shared<DisplaySnapshotTest>(10);

    string testKey;

    while (true) {
        cout << "Enter test key (e.g., 1 for group tests, 2.5 for a specific test) or '0' to quit: ";
        cin >> testKey;

        // 세션 종료
        if (testKey == "0") {
            cout << "Exiting the test runner. Goodbye!" << endl;
            break;
        }

        try {
            size_t pos = testKey.find('.');
            int groupNumber = stoi(testKey.substr(0, pos)); // 그룹 번호 추출

            // 그룹 존재 확인
            if (testGroups.find(groupNumber) == testGroups.end()) {
                throw out_of_range("No test group found for groupNumber: " + to_string(groupNumber));
            }

            if (pos == string::npos) {
                // 자연수 입력: 그룹 전체 테스트 실행
                testGroups[groupNumber]->execute();
            }
            else {
                // 특정 테스트 실행 (e.g., 1.5)
                int testId = stoi(testKey.substr(pos + 1));
                testGroups[groupNumber]->execute(testId);
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
