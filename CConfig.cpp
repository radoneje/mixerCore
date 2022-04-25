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
extern "C" {
}




CConfig::CConfig() {};

int CConfig::GetConfig(const char *fileNname, const char** str) {

    std::ifstream cFile();
    cFile.open(fileNname );
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
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            std::cout << name << " " << value << '\n';
        }

    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
}

