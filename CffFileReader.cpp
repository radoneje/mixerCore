
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
void CffFileReader::work(std::string fileid, std::string fileurl, CEvent *pEvent)
{
    AVFormatContext *ctx_format = nullptr;
    AVCodecContext *ctx_codec = nullptr;
    AVCodecContext *ctx_aud_codec = nullptr;
    AVStream *vid_stream = nullptr;
    const AVCodec  *codec;
    int ret;
    int stream_idx;
    

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

    for (int i = 0; i < ctx_format->nb_streams; i++) {
        if (ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && stream_idx<0) {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            //break;
        }
    }
    if (vid_stream == nullptr) {
        CConfig::error("FILE READER ERROR - aavformat VIDEO", fileid, fileurl);
        return;
    }
    codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
    if (!codec) {
         CConfig::error("FILE READER ERROR - codec not found", fileid, fileurl);
        return ;
    }


    CConfig::log("FILE READER READY ", fileid, fileurl);
};
