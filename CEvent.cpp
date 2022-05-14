//
// Created by Denis Shevchenko on 14.05.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <thread>
#include <mutex>
#include <vector>

#include "CEvent.h"
CEvent::CEvent(std::string eventid) {
    _eventid=eventid;
    stop= false;
}