//
// Created by Denis Shevchenko on 25.04.2022.
//

#ifndef MIXERCORE_CCONFIG_H
#define MIXERCORE_CCONFIG_H


class CConfig {
public: CConfig();
    static int GetConfig(const char *name, string* str);
};


#endif //MIXERCORE_CCONFIG_H
