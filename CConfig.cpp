//
// Created by Denis Shevchenko on 25.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <charconv>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <locale>



#include "httplib.h"
#include "CConfig.h"




CConfig::CConfig() {};

 int CConfig::MAX_FACES=7;
 int CConfig::WIDTH=1280;
 int CConfig::HEIGHT=720;

 int CConfig::FRAMERATE=30;
 int CConfig::MIXER_BITRATE=1024*1024*1.5;
 int CConfig::MIXER_GOP=30;
 int CConfig::HTTP_SERVER_PORT=8090;

 std::string CConfig::RTMP_MAIN="rtmp://wowza02.onevent.online/live/";
 std::string CConfig::CONTROL_SERVER="localhost";
 int CConfig::CONTROL_SERVER_PORT=3000;

 void CConfig::_HTTPgetRequest(std::string target){

     std::string srv="http://"+CONTROL_SERVER+":"+std::to_string(CONTROL_SERVER_PORT);
     target="/api/v1/"+target;

     httplib::Client cli(srv);
     auto res = cli.Get(target.c_str());


    return;


};

int CConfig::getGlobalValues(){
       try {
            std::string buf;
            GetConfig("MAX_FACES", buf);
            MAX_FACES= atoi(buf.c_str());

            GetConfig("WIDTH", buf);
            WIDTH= atoi(buf.c_str());

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

           GetConfig("HTTP_SERVER_PORT", buf);
           HTTP_SERVER_PORT= atoi(buf.c_str());

           GetConfig("CONTROL_SERVER", buf);
           CONTROL_SERVER= buf;

           GetConfig("CONTROL_SERVER_PORT", buf);
           CONTROL_SERVER_PORT= atoi(buf.c_str());

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
void CConfig::notifyControl(std::string action, std::string eventid, std::string *param1, std::string *param2,
                                   std::string *param3) {
    std::string url=action+"/"+eventid;
    if(param1!= nullptr){
        url+= "/";
        url+= param1->c_str();
    }
    if(param2!= nullptr){
        url+= "/";
        url+= param2->c_str();
    }
    if(param3!= nullptr){
        url+= "/";
        url+= param3->c_str();
    }
    std::thread requestThread(_HTTPgetRequest, url);
    requestThread.detach();

}



