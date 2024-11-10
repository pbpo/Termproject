//
//  LanguageSupport.hpp
//  Termproject
//
//  Created by cho on 10/20/24.
//
#pragma once

#include <string>
#include <map>
#include "Utility.hpp"

class LanguageSupport {
private:
    Language currentLanguage;
    std::map<std::string, std::string> messagesEnglish;
    std::map<std::string, std::string> messagesKorean;
    LanguageSupport();
    static LanguageSupport* instance;

public:
    static LanguageSupport* getInstance();
    void setLanguage(Language language);
    std::string getMessage(const std::string& key);
};
