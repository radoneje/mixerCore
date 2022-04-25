//
// Created by Denis Shevchenko on 25.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>

#include "CConfig.h"





CConfig::CConfig() {};

int CConfig::GetConfig(const char *name, std::string str) {

    std::ifstream cFile("/etc/mixerCore/mixerCore.conf");

   // if(! config_read_file(&cfg, "/etc/MixerCore/mixerCore.conf"))
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                       line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto key = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            std::string sKey(name);

            if(sKey.compare(key)==0) {
                str = value;
                std::cout<< "value: "<< value <<std::end;
                return 0;
            }
        }

    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
    return -1;
}

