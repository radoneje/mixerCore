//
// Created by Denis Shevchenko on 10.05.2022.
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

#include "CffmpegStreamer.h"

using namespace std;
CffmpegStreamer::CffmpegStreamer(const std::string &eventid) {

}
int CffmpegStreamer::init() {
    cout<<"CffmpegStreamer init"<<endl;
}
