//
// Created by Denis Shevchenko on 06.05.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

extern "C" {
// Get declaration for f(int i, char c, float x)
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}

#include "CFFreader.h"

CFFreader::CFFreader(){};
void CFFreader::work(const std::string url, Data *pData){//, Data &pData){
    std::cout <<"in Worker"<<  pData->width << " " <<url << std::endl;
    AVFormatContext *ctx_format = nullptr;
    AVCodecContext *ctx_codec = nullptr;
    AVCodec *codec = nullptr;
    AVFrame *frame = av_frame_alloc();
    int stream_idx;
    const char *fin =url.c_str();//"/tmp/vcbr.mp4";// url.c_str();;
    AVStream *vid_stream = nullptr;
    AVPacket *pkt = av_packet_alloc();
    int ret;
    int sts;
    struct SwsContext *sws_ctx = NULL;

    if (int ret = avformat_open_input(&ctx_format, fin, nullptr, nullptr) != 0) {
        std::cout <<"ERROR avformat  " << 1 << std::endl;
        return ;
    }
    std::cout << "avformat finding codec..." << std::endl;
    if (avformat_find_stream_info(ctx_format, nullptr) < 0) {
        std::cout <<"ERROR avformat  " << 2 << std::endl;
        return ; // Couldn't find stream information
    }

    av_dump_format(ctx_format, 0, fin, false);

    for (int i = 0; i < ctx_format->nb_streams; i++)
        if (ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            break;
        }

    if (vid_stream == nullptr) {
        std::cout<<"ERROR avformat  " << 4 << std::endl;
        return ;
    }
    std::cout << " framerate: " << vid_stream->avg_frame_rate.num << " " << vid_stream->avg_frame_rate.den << std::endl;
    codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    ctx_codec = avcodec_alloc_context3(codec);

    if (avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar) < 0)
        std::cout << 512;
    if (avcodec_open2(ctx_codec, codec, nullptr) < 0) {
        std::cout << 5;
        return -1;
    }
    sws_ctx = sws_getContext(ctx_codec->width,
                             ctx_codec->height,
                             ctx_codec->pix_fmt,
                             ctx_codec->width,
                             ctx_codec->height,
                             AV_PIX_FMT_RGB24,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);
    std::cout << 94 << std::endl;
    return ;
}

