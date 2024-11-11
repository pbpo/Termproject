#include <iostream>
#include "MainProgram.hpp"
#include "TestRunner.hpp"


using namespace std;

int main() {
    
    int choice;

    while (true) {
        cout << "Enter 0 to quit, 1 to run tests, or 2 to run the program: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting the application. Goodbye!" << endl;
            break;
        }
        else if (choice == 1) {
            cout << "Entering test mode..." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            runTest();
        }

        else if (choice == 2) {
            cout << "Running main program..." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            initializeSystem();
        }
        else {
            cout << "Invalid choice. Please enter 0, 1, or 2." << endl;
        }
    }
    return 0;
}
