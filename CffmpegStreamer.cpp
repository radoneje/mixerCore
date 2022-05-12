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
#include <functional>
extern "C" {
// Get declaration for f(int i, char c, float x)
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>

}

#include "CffmpegStreamer.h"
#include "Ccmd.h"

using namespace std;
CffmpegStreamer::CffmpegStreamer() {

}
int CffmpegStreamer::init() {
    cout<<"CffmpegStreamer init"<<endl;
    return  1;
}
void CffmpegStreamer::startStream(const std::string eventid, unsigned char * image, const std::function<void(int)>* startCallback,  const std::function<void(int)>* EndCallback){

    cout<<"CffmpegStreamer thread: "<< eventid <<endl;
}
