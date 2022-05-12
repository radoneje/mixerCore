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
void CffmpegStreamer::encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    int ret;
    /* send the frame to the encoder */
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "error during encoding\n");
            exit(1);
        }
        printf("encoded frame %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}
void CffmpegStreamer::startStream(const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, std::map<std::string, SstreamData *> *pStreamers){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    AVCodec *codec;
         AVCodecContext *codecContext= NULL;
         int i, out_size, size, x, y, outbuf_size, ret;
         FILE *f;
         AVFrame *picture;
         AVPacket *pkt;
         uint8_t *outbuf, *picture_buf;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

         printf("Video encoding\n");

         std::string filename="/var/www/mixerControl/public/1.mp4";
    f = fopen(filename.c_str(), "wb");
    if (!f) {
        fprintf(stderr, "could not open %s\n", filename.c_str());
        fprintf(stderr, "could not open codec\n");
        EndCallback( eventid, pStreamers);
        return;
        exit(1);
    }

    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
         if (!codec) {
                 fprintf(stderr, "codec not found\n");
             EndCallback( eventid, pStreamers);
             return;
             }
    codecContext = avcodec_alloc_context3(codec);
    picture = av_frame_alloc();
    pkt = av_packet_alloc();
    if (!pkt)
    {
        fprintf(stderr, "Error av_packet_alloc\n");
        EndCallback( eventid, pStreamers);
        return;
    }
    codecContext->bit_rate = 400000;
    /* resolution must be a multiple of two */
    codecContext->width = 1280;
    codecContext->height = 720;
    /* frames per second */
    codecContext->time_base = (AVRational){1, 30};
    codecContext->framerate = (AVRational){30, 1};
    codecContext->gop_size = 30; /* emit one intra frame every 30 frames */
    codecContext->max_b_frames=1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    av_dict_set(&This->opts, "vprofile", "baseline", 0)
    av_opt_set(codecContext->priv_data, "preset", "slow", 0);

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        EndCallback( eventid, pStreamers);
        return;
    }

    picture->format = codecContext->pix_fmt;
    picture->width  = codecContext->width;
    picture->height = codecContext->height;
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "could not alloc the frame data\n");
        EndCallback(eventid, pStreamers);
        return;
    }
    for(i=0;i<30*10;i++) {
        av_init_packet(pkt);


    }
    std::cout <<"encode all frames " <<std::endl;

    encode(codecContext, NULL, pkt, f);

    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&codecContext);
    av_frame_free(&picture);
    av_packet_free(&pkt);

    EndCallback(eventid, pStreamers);

    //startCallback(eventid, pStreamers);
}
