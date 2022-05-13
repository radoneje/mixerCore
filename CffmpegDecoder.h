//
// Created by Denis Shevchenko on 13.05.2022.
//

#ifndef MIXERCORE_CFFMPEGDECODER_H
#define MIXERCORE_CFFMPEGDECODER_H

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
#include "SstreamData.h"

class CffmpegDecoder {
private:
public:
    static void startDecode( const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, streamersDataType *pStreamers);

};


#endif //MIXERCORE_CFFMPEGDECODER_H
