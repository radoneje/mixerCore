
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

extern "C"
{
// Get declaration for f(int i, char c, float x)
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}

#include "CffFileReader.h"
#include "CConfig.h"

CffFileReader::CffFileReader()
{
}
long CffFileReader::nowTime()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch());
    return ms.count();
}
void CffFileReader::work(std::string fileid, std::string fileurl, CEvent *pEvent)
{
    AVFormatContext *ctx_format = nullptr;
    AVCodecContext *ctx_codec = nullptr;
    AVCodecContext *ctx_aud_codec = nullptr;
    AVStream *vid_stream = nullptr;
    AVPacket *pkt = av_packet_alloc();
    const AVCodec *codec;
    int ret;
    int stream_idx = -1;
    int ii = 0;

    if (int ret = avformat_open_input(&ctx_format, fileurl.c_str(), nullptr, nullptr) != 0)
    {
        CConfig::error("FILE READER ERROR - avformat_open_input", fileid, fileurl);
        return;
    }

    ret = avformat_find_stream_info(ctx_format, nullptr);
    if (ret < 0)
    {
        CConfig::error("FILE READER ERROR - avformat_find_stream_info", fileid, fileurl);
        return; // Couldn't find stream information
    }
    av_dump_format(ctx_format, 0, fileurl.c_str(), false);

    for (int i = 0; i < ctx_format->nb_streams; i++)
    {
        if (ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && stream_idx < 0)
        {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            // break;
        }
    }
    if (vid_stream == nullptr)
    {
        CConfig::error("FILE READER ERROR - aavformat VIDEO", fileid, fileurl);
        return;
    }
    codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
    if (!codec)
    {
        CConfig::error("FILE READER ERROR - codec not found", fileid, fileurl);
        return;
    }
    ctx_codec = avcodec_alloc_context3(codec);

    if (avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar) < 0)
    {
        CConfig::error("FILE READER ERROR - avcodec_parameters_to_context", fileid, fileurl);
        return;
    }
    if (avcodec_open2(ctx_codec, codec, nullptr) < 0)
    {
        CConfig::error("FILE READER ERROR - avcodec_open2", fileid, fileurl);
        return;
    }
    // задержка на частоту кадров
    int frameDur = 0;
    if (vid_stream->avg_frame_rate.num > 0)
        frameDur = (vid_stream->avg_frame_rate.den * 1000) / vid_stream->avg_frame_rate.num;
    long lastFrameTime = 0;
    CConfig::log("FILE READER frameDur 2", frameDur);

    //////////

    AVFrame *frame = av_frame_alloc();
    AVFrame *pRGBFrame = av_frame_alloc();
    AVFrame *pRGBFrame_preview = av_frame_alloc();

    pRGBFrame->format = AV_PIX_FMT_RGB24;
    pRGBFrame->width = CConfig::WIDTH * 0.75;
    pRGBFrame->height = CConfig::HEIGHT * 0.75;

    pRGBFrame_preview->format = AV_PIX_FMT_RGB24;
    pRGBFrame_preview->width = CConfig::WIDTH * 0.25;
    pRGBFrame_preview->height = CConfig::HEIGHT * 0.25;

    int sts = av_frame_get_buffer(pRGBFrame, 0);
    if (sts < 0)
    {

        CConfig::error("FILE READER ERROR - av_frame_get_buffer", fileid, fileurl);
        return;
    }
    int sts_preview = av_frame_get_buffer(pRGBFrame_preview, 0);
    if (sts_preview < 0)
    {
        CConfig::error("FILE READER ERROR - av_frame_get_buffer", fileid, fileurl);
        return; // Error!
    }
    if (!pEvent->onVideoLoaded(fileid))
        return;
    while (av_read_frame(ctx_format, pkt) >= 0)
    {

        /* while(pEvent->videoFileReaders.at(fileid)->isPaused){
             std::this_thread::sleep_for(std::chrono::milliseconds(300));
             if(pEvent->stop)
                  break;
         }*/
        if (pEvent->stop)
            break;
        ret = avcodec_send_packet(ctx_codec, pkt);
        if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            std::cout << "avcodec_send_packet: " << ret << " " << AVERROR(EAGAIN) << " " << AVERROR_EOF << std::endl;
            break;
        }
        while (ret >= 0)
        {
            ret = avcodec_receive_frame(ctx_codec, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            {
                // std::cout << "avcodec_receive_frame: " << ret << std::endl;

                break;
            }
            ii++;

            int dstStride = (int)(CConfig::WIDTH * 0.75 * 3);
            struct SwsContext *sws_ctx = sws_getContext(
                frame->width,
                frame->height,
                ctx_codec->pix_fmt,
                CConfig::WIDTH * 0.75,
                CConfig::HEIGHT * 0.75,
                AV_PIX_FMT_RGB24,
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL);
            struct SwsContext *sws_ctx_preview = sws_getContext(
                frame->width,
                frame->height,
                ctx_codec->pix_fmt,
                CConfig::WIDTH * 0.25,
                CConfig::HEIGHT * 0.25,
                AV_PIX_FMT_RGB24,
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL);
            sts = sws_scale(sws_ctx,         // struct SwsContext* c,
                            frame->data,     // const uint8_t* const srcSlice[],
                            frame->linesize, // const int srcStride[],
                            0,               // int srcSliceY,
                            frame->height,   // int srcSliceH,
                            pRGBFrame->data, // uint8_t* const dst[],
                            &dstStride);     // const int dstStride[]);
                                             // задержка на частоту кадров
            dstStride = (int)(CConfig::WIDTH * 0.25 * 3);

            sts_preview = sws_scale(sws_ctx_preview,         // struct SwsContext* c,
                                    frame->data,             // const uint8_t* const srcSlice[],
                                    frame->linesize,         // const int srcStride[],
                                    0,                       // int srcSliceY,
                                    frame->height,           // int srcSliceH,
                                    pRGBFrame_preview->data, // uint8_t* const dst[],
                                    &dstStride);

            long thisFrameTime = lastFrameTime + frameDur;
            if (thisFrameTime > nowTime())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(thisFrameTime - nowTime()));
            }
            lastFrameTime = nowTime();

            std::size_t size =
                pRGBFrame->width * pRGBFrame->height * (pRGBFrame->linesize[0] / pRGBFrame->width) *
                sizeof(unsigned char);

            memcpy(pEvent->imageData[CConfig::MAX_FACES].fullImageData, pRGBFrame->data[0], size);

            size =
                pRGBFrame_preview->width * pRGBFrame_preview->height * (pRGBFrame_preview->linesize[0] / pRGBFrame_preview->width) *
                sizeof(unsigned char);
            memcpy(pEvent->imageData[CConfig::MAX_FACES].previewImageData, pRGBFrame_preview->data[0], size);
            pEvent->imageData[CConfig::MAX_FACES].frameNumber = ctx_codec->frame_number;
        }
    }
    pEvent->onVideoEnd(fileid);

    av_frame_free(&frame);
    av_frame_free(&pRGBFrame);
    av_frame_free(&pRGBFrame_preview);
    av_packet_unref(pkt);
    avcodec_free_context(&ctx_codec);
    avformat_close_input(&ctx_format);

    CConfig::log("FILE READER READY ", fileid, fileurl);
};
