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

CFFreader::CFFreader(){
    dt.width=0;
    dt.height=0;
    dt.frameNumber=-1;
    dt.numChannels=0;

};
 long CFFreader::nowTime() {
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );
    return ms.count();
}
void CFFreader::work(const std::string url, Data *pData, std::mutex *pLocker){//, Data &pData){
    std::cout <<"in Worker"<<  pData->width << " " <<url << std::endl;
    AVFormatContext *ctx_format = nullptr;
    AVCodecContext *ctx_codec = nullptr;
    AVCodec *codec = nullptr;

    int stream_idx;
    const char *fin =url.c_str();//"/tmp/vcbr.mp4";// url.c_str();;
    AVStream *vid_stream = nullptr;
    AVPacket *pkt = av_packet_alloc();
    int ret;

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
        std::cout << "ERROR codec not found" << std::endl;
        return ;
    }
    ctx_codec = avcodec_alloc_context3(codec);

    if (avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar) < 0)
        std::cout << 512;
    if (avcodec_open2(ctx_codec, codec, nullptr) < 0) {
        std::cout << "ERROR avcodec_open2"<< std::endl;
        return ;
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


    // задержка на частоту кадров
    int frameDur = (vid_stream->avg_frame_rate.den * 1000) / vid_stream->avg_frame_rate.num;
    long lastFrameTime = 0;

   // finalFrameData_lock.lock();
    {
        std::lock_guard<std::mutex> lockGuard(*pLocker);
        pData->width = 0;
        pData->height = 0;
        pData->frameNumber = -1;
    }
   // finalFrameData_lock.unlock();

    int ii = 0;
    std::cout<< "av_lib ready to the read" << std::endl;
    AVFrame *pRGBFrame = av_frame_alloc();
    pRGBFrame->format = AV_PIX_FMT_RGB24;
    pRGBFrame->width = ctx_codec->width;
    pRGBFrame->height = ctx_codec->height;
    int sts = av_frame_get_buffer(pRGBFrame, 0);

    //////////
    while (av_read_frame(ctx_format, pkt) >= 0) {

        if (pkt->stream_index == stream_idx) {
            //Allocate frame for storing image converted to RGB.
            ////////////////////////////////////////////////////////////////////////////
            av_frame_free(&pRGBFrame);
            *pRGBFrame = av_frame_alloc();
            pRGBFrame->format = AV_PIX_FMT_RGB24;
            pRGBFrame->width = ctx_codec->width;
            pRGBFrame->height = ctx_codec->height;
            int sts = av_frame_get_buffer(pRGBFrame, 0);

            AVFrame *frame = av_frame_alloc();

            if (sts < 0) {
                std::cout <<"ERROR av_frame_get_buffer" << 4444 << std::endl;
                return ;  //Error!
            }

            int ret = avcodec_send_packet(ctx_codec, pkt);
            if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                std::cout << "avcodec_send_packet: " << ret << " " << AVERROR(EAGAIN) << " " << AVERROR_EOF<<std::endl;

                av_frame_free(&pRGBFrame);
                av_frame_free(&frame);
                break;
            }
            while (ret >= 0) {
                std::lock_guard<std::mutex> lockGuard(*pLocker);

                ret = avcodec_receive_frame(ctx_codec, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    //std::cout << "avcodec_receive_frame: " << ret << std::endl;

                    break;
                }
                ii++;

                int64_t pts = av_rescale(frame->pts, 1000000, AV_TIME_BASE);
                int64_t now = av_gettime_relative();//- frame->start;

                sts = sws_scale(sws_ctx,                //struct SwsContext* c,
                                frame->data,            //const uint8_t* const srcSlice[],
                                frame->linesize,        //const int srcStride[],
                                0,                      //int srcSliceY,
                                frame->height,          //int srcSliceH,
                                pRGBFrame->data,        //uint8_t* const dst[],
                                pRGBFrame->linesize);   //const int dstStride[]);
                if (sts != frame->height) {
                    std::cout << "sts != frame->height " << std::endl;
                    return ;  //Error!
                }

                // задержка на частоту кадров
                long thisFrameTime = lastFrameTime + frameDur;
                if (thisFrameTime > nowTime()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(thisFrameTime - nowTime()));
                }
                lastFrameTime = nowTime();

                {

                    pData->width = pRGBFrame->width;
                    pData->height = pRGBFrame->height;
                    pData->pixels = pRGBFrame->data[0];
                    pData->linesize = pRGBFrame->linesize[0];
                    pData->frameNumber = ctx_codec->frame_number;

                }
            }

            av_frame_free(&frame);

        }
    }
    ///////////
    {
        std::lock_guard<std::mutex> lockGuard(*pLocker);
        if(pData->width>720 && pData->width<1921)
            free(pData->pixels);
        pData->width = 0;
        pData->height = 0;
        pData->frameNumber = -1;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(frameDur));

    av_packet_unref(pkt);

    avcodec_free_context(&ctx_codec);
    avformat_close_input(&ctx_format);

    return ;
}

