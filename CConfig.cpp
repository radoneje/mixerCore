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
#include <algorithm>
#include <cctype>
#include <locale>

#include "CConfig.h"




CConfig::CConfig() {};

 int CConfig::MAX_FACES=7;
 int CConfig::WIDTH=1280;
 int CConfig::HEIGHT=720;
 std::string CConfig::RTMP_MAIN="rtmp://wowza02.onevent.online/live/";
 int CConfig::FRAMERATE=30;
 int CConfig::MIXER_BITRATE=1024*1024*1.5;
 int CConfig::MIXER_GOP=30;

int CConfig::getGlobalValues(){
       try {
            std::string buf;
            GetConfig("MAX_FACES", buf);
            MAX_FACES= atoi(buf.c_str());

            GetConfig("WIDTH", buf);
            WIDTH= atoi(buf.c_str());
            log("WIDTH",WIDTH );

            GetConfig("HEIGHT", buf);
            HEIGHT= atoi(buf.c_str());

            GetConfig("FRAMERATE", buf);
            FRAMERATE= atoi(buf.c_str());

            GetConfig("MIXER_BITRATE", buf);
            MIXER_BITRATE= atoi(buf.c_str());

            GetConfig("MIXER_GOP", buf);
            MIXER_GOP= atoi(buf.c_str());

            GetConfig("RTMP_MAIN", buf);
            RTMP_MAIN= buf;

        }
        catch (...){
            std::cout<< "error read config file"<<std::endl;
            return 1;
        }
        return 0;
}

int CConfig::GetConfig(const char *name, std::string &str) {

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
                trim(value);
                str = value;
                log("load config:",name, value);
                return 0;
            }
        }

    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
    return -1;
}

