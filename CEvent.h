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

class CEvent {
private:
    std::string _eventid;
public:
    struct SImageData{
        unsigned char * previewImageData;
        unsigned char * fullImageData
    };
    CEvent(std::string eventid);
    std::mutex locker;
    std::thread *thread;
    long  startTime;
    unsigned char* mainImageData; // главная картинка
    std::vector<SImageData>  imageData; // картинка по входам
    bool  stop;
};


#endif //MIXERCORE_CEVENT_H
