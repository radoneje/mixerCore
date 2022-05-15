//
// Created by Denis Shevchenko on 10.05.2022.
//

#ifndef MIXERCORE_CFFMPEGSTREAMER_H
#define MIXERCORE_CFFMPEGSTREAMER_H
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


#include "CEvent.h"

class CffmpegStreamer {
private:
    static double r2d(AVRational r);
public:
    CffmpegStreamer();
    static void startStream( const std::string eventid, CEvent *pEvent,  std::function<void(std::string )> onStart,   std::function<void(std::string)> onEnd);
    int init();
};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
