//
// Created by Denis Shevchenko on 14.05.2022.
//

#ifndef MIXERCORE_CEVENT_H
#define MIXERCORE_CEVENT_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <thread>
#include <mutex>
#include <vector>
#include <map>


class CEvent {
private:
    std::string _eventid;


public:

    struct SImageData{
        unsigned char * previewImageData;
        unsigned char * fullImageData;
        std::string itemid;
        long long frameNumber;
    };
    struct SInputData{
        long time_start;
         int number;
        std::string spkid;
        bool isActive;
    };
    struct SVideoFileData{
        std::string fileid;
        bool isPaused;
        bool isReady;
        bool islooped;
        bool isRequestToStart;
    };
   

    CEvent(std::string eventid);
    std::mutex locker;
    std::thread *thread;
    long  startTime;
    unsigned char* mainImageData; // главная картинка
    std::vector<SImageData>  imageData; // картинка по входам
    bool  stop;
    std::vector<int> activeInputs;
    SImageData loadDefaultInput(int inputNo);
    bool onInputStart(int inputNo);
    void onInputEnd(int inputNo);
    void showPres(unsigned char * data, std::string itemid);
    std::map<int,SInputData*> inputs;

    bool onVideoLoaded(std::string fileid, bool islooped);
    void onVideoEnd(std::string fileid);
    void stopAllVideos();
    std::map<std::string, SVideoFileData*> videoFileReaders;
    void onVideoFilePause(std::string fileid, bool pause);

   
};


#endif //MIXERCORE_CEVENT_H
