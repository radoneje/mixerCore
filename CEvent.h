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
public:
    CEvent(std::string eventId);
    std::string eventid;
    int inputNo;
    std::thread *thread;
};


#endif //MIXERCORE_CEVENT_H
