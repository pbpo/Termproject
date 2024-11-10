//
//  InputHandler.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

// InputHandler.hpp
#pragma once

#include <string>
#include <variant>

enum class InputType {
    STRING,
    INT,
    DOUBLE
};

class InputHandler {
public:
    static std::variant<std::string, int, double> getInput(const std::string& prompt, InputType type);
};
