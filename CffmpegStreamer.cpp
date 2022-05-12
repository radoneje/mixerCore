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
#include <map>


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
#include "SstreamData.h"


using namespace std;
CffmpegStreamer::CffmpegStreamer() {

}
int CffmpegStreamer::init() {
    cout<<"CffmpegStreamer init"<<endl;
    return  1;
}
void CffmpegStreamer::startStream(const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, std::map<std::string, SstreamData *> *pStreamers){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    startCallback( eventid, pStreamers);

    AVCodec *codec;
         AVCodecContext *c= NULL;
         int i, out_size, size, x, y, outbuf_size;
         FILE *f;
         AVFrame *picture;
         uint8_t *outbuf, *picture_buf;
         printf("Video encoding\n");

    //startCallback(eventid, pStreamers);
}
