//
// Created by Denis Shevchenko on 10.05.2022.
//

#ifndef MIXERCORE_CFFMPEGSTREAMER_H
#define MIXERCORE_CFFMPEGSTREAMER_H
#include <functional>
#include "SstreamData.h"


class CffmpegStreamer {
public:
    CffmpegStreamer();
    static void startStream( const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, streamersDataType *pStreamers);
    int init();
};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
