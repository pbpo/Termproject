// InputHandler.cpp
#include "InputHandler.hpp"
#include "SystemStatus.hpp"
#include <iostream>
#include <limits>
#include <sstream>

std::variant<std::string, int, double> InputHandler::getInput(const std::string& prompt, InputType type) {
    while (true) {
        std::cout << prompt;

        // Clear input stream if failed
        if (std::cin.fail()) {
            std::cin.clear(); // Clear fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input
        }

        std::string input;
        std::getline(std::cin, input);

        if (input == "/") {
            // Display system status
            SystemStatus::getInstance()->displayStatus();
            continue;
        } else if (input.empty()) {
            std::cout << "Input cannot be empty. Please try again." << std::endl;
            continue;
        }

        std::istringstream iss(input);
        if (type == InputType::STRING) {
            return input;
        } else if (type == InputType::INT) {
            int value;
            if (iss >> value && iss.eof()) {
                return value;
            } else {
                std::cout << "Invalid integer input. Please try again." << std::endl;
                continue;
            }
        } else if (type == InputType::DOUBLE) {
            double value;
            if (iss >> value && iss.eof()) {
                return value;
            } else {
                std::cout << "Invalid number input. Please try again." << std::endl;
                continue;
            }
        } else {
            std::cout << "Unsupported input type." << std::endl;
            continue;
        }
    }
}
