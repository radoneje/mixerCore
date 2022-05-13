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
                       FILE *outfile);
    static double r2d(AVRational r);
    static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);
    /*typedef struct OutputStream {
        AVStream *st;
        AVCodecContext *enc;

        ///* pts of the next frame that will be generated
        int64_t next_pts;
        int samples_count;

        AVFrame *frame;
        AVFrame *tmp_frame;

        AVPacket *tmp_pkt;

        float t, tincr, tincr2;

        struct SwsContext *sws_ctx;
        struct SwrContext *swr_ctx;
    } OutputStream;*/
public:
    CffmpegStreamer();
    static void startStream( const std::string eventid, unsigned char * image,std::mutex *locker,  std::function<void(std::string )> onStart,   std::function<void(std::string)> onEnd);
    int init();


};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
