//
// Created by Denis Shevchenko on 25.04.2022.
//

#ifndef MIXERCORE_CCONFIG_H
#define MIXERCORE_CCONFIG_H

#include "CSettings.h"


class CConfig {
private:
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

// trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

// trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
public: CConfig();
    static int GetConfig(const char *name, std::string &str);
    static int getGlobalValues();
};


#endif //MIXERCORE_CCONFIG_H
