//
// Created by Denis Shevchenko on 25.04.2022.
//

#ifndef MIXERCORE_CCONFIG_H
#define MIXERCORE_CCONFIG_H


class CConfig {
public: CConfig(char* name, char& str);
    static int GetConfig();
};


#endif //MIXERCORE_CCONFIG_H
