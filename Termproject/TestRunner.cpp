#include <iostream>
#include <unordered_map>
#include <memory>

#include "TestGroup.hpp"
// ��ɺ� test Ŭ���� import
#include "AdminMenuTest.hpp"

using namespace std;

void runTest() {
    unordered_map<int, shared_ptr<TestGroup>> testGroups;

    // �׽�Ʈ �׷� �߰� (����)
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

        // ���� ����
        if (testKey == "0") {
            cout << "Exiting the test runner. Goodbye!" << endl;
            break;
        }

        try {
            size_t pos = testKey.find('.');
            int groupNumber = stoi(testKey.substr(0, pos)); // �׷� ��ȣ ����

            // �׷� ���� Ȯ��
            if (testGroups.find(groupNumber) == testGroups.end()) {
                throw out_of_range("No test group found for groupNumber: " + to_string(groupNumber));
            }

            if (pos == string::npos) {
                // �ڿ��� �Է�: �׷� ��ü �׽�Ʈ ����
                testGroups[groupNumber]->execute();
            }
            else {
                // Ư�� �׽�Ʈ ���� (e.g., 1.5)
                int testId = stoi(testKey.substr(pos + 1));
                testGroups[groupNumber]->execute(testId);
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
