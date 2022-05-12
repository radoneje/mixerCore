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

#include "SstreamData.h"



class CffmpegStreamer {
private:
    static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                       FILE *outfile)
public:
    CffmpegStreamer();
    static void startStream( const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, streamersDataType *pStreamers);
    int init();

};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
