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
#include <libavutil/timestamp.h>


}


#include "CffmpegStreamer.h"
#include "Ccmd.h"
#include "SstreamData.h"
//#include "CConfig.h"


using namespace std;


 double CffmpegStreamer::r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}
CffmpegStreamer::CffmpegStreamer() {

}
int CffmpegStreamer::init() {
    cout<<"CffmpegStreamer init"<<endl;
    return  1;
}

void CffmpegStreamer::startStream(const std::string eventid, unsigned char * image,std::mutex *locker,  std::function<void(std::string)> onStart,   std::function<void(std::string)> onEnd){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    av_register_all();
    avcodec_register_all();

    AVStream *out_stream;
    AVOutputFormat * oformat = nullptr;
    AVFormatContext * ofmt_ctx = NULL;
    const AVCodec *encoder;

    AVCodecContext *enc_ctx;
    AVPacket *pkt;
    AVFrame *frame;
    SwsContext *sws_ctx = NULL;
    AVDictionary *opts = nullptr;

    std::string outUrl=RTMP_MAIN;
    outUrl.append(eventid);

    oformat = av_guess_format("flv", "test.mp4", NULL);//, "test.mp4", nullptr);
    if (!oformat)
    {
        std::cout << "can't create output format" << std::endl;
        return;
    }
    oformat->video_codec = AV_CODEC_ID_H264;


    int ret;


    avformat_alloc_output_context2(&ofmt_ctx, oformat, NULL, outUrl.c_str());
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return ;
    }


    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    out_stream->id = 0;
    out_stream->time_base = AVRational { 1, 30};
    out_stream->avg_frame_rate = AVRational{ 30, 1 };

    if (!out_stream) {
        av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
        return ;
    }

    //encoder = avcodec_find_encoder_by_name(codec_name.c_str());

    encoder=  avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!encoder) {
        fprintf(stderr, "Codec AV_CODEC_ID_H264 not found\n");
        return ;
    }
    enc_ctx = avcodec_alloc_context3(encoder);
    /* put sample parameters */
    enc_ctx->bit_rate = MIXER_BITRATE;
    /* resolution must be a multiple of two */
    enc_ctx->width = WIDTH;
    enc_ctx->height = HEIGHT;
    /* frames per second */
    enc_ctx->time_base = (AVRational){1, FRAMERATE};
    enc_ctx->framerate = (AVRational){FRAMERATE, 1};

    enc_ctx->gop_size = MIXER_GOP;
    enc_ctx->max_b_frames = 1;
    enc_ctx->profile=FF_PROFILE_H264_BASELINE;
    enc_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    av_dict_set(&opts, "preset", "fast", 0);
    av_dict_set(&opts, "tune", "zerolatency", 0);

    ret = avcodec_open2(enc_ctx, encoder, &opts);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream \n" );
        return ;
    }

    ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream \n");
        return ;
    }
    out_stream->time_base = enc_ctx->time_base;
    out_stream->avg_frame_rate=enc_ctx->framerate;
    out_stream->time_base=enc_ctx->time_base;

    AVDictionary* options = nullptr;
    av_dict_set( &options, "movflags", "frag_keyframe+empty_moov+default_base_moof", 0 );

    ret=avio_open(&ofmt_ctx->pb, outUrl.c_str(), AVIO_FLAG_WRITE);// , NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error avio_open\n");
        return;
    }
    ret = avformat_write_header(ofmt_ctx, 0);
    if (ret < 0) {
        fprintf(stderr, "Error avformat_write_header\n");
        return;
    }

    pkt = av_packet_alloc();
    frame = av_frame_alloc();
    frame->format = enc_ctx->pix_fmt;
    frame->width  = enc_ctx->width;
    frame->height = enc_ctx->height;
    ret = av_frame_get_buffer(frame, 32);

    sws_ctx = sws_getContext(enc_ctx->width,
                             enc_ctx->height,
                             AV_PIX_FMT_RGB24,
                             enc_ctx->width,
                             enc_ctx->height,
                             AV_PIX_FMT_YUV420P,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);

    long long startTime = av_gettime();
    av_dump_format(ofmt_ctx, 0, outUrl.c_str(), 1);
    long long i=0, j=0;
        while(true ||  i<60) {
            i++;
            j++;
        fflush(stdout);
        ret = av_frame_make_writable(frame);

       int linesize=WIDTH*3;
       locker->lock();
            ret = sws_scale(sws_ctx,                //struct SwsContext* c,
                            &image,            //const uint8_t* const srcSlice[],
                            &linesize,        //const int srcStride[],
                            0,                      //int srcSliceY,
                            frame->height,          //int srcSliceH,
                            frame->data,        //uint8_t* const dst[],
                            frame->linesize);   //const int dstStride[]);
       locker->unlock();
        frame->pts +=1000/enc_ctx->time_base.den;//   av_rescale_q( 1, enc_ctx->time_base, out_stream->time_base);
            if(frame->pts<0) {
                frame->pts = 0;
                startTime = av_gettime();
            }
            int64_t now_time = av_gettime();// - startTime;
            int must=(1000)*frame->pts;
            int  fact = now_time-startTime;
            if(must>fact) {
                av_usleep(must - fact);
            }



        ret = avcodec_send_frame(enc_ctx, frame);
        if (ret < 0) {
            fprintf(stderr, "Error sending a frame to the encoder: %s\n", i);
            return;
        }
        while (ret >= 0) {

            ret = avcodec_receive_packet(enc_ctx, pkt);

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else  if (ret < 0) {
                fprintf(stderr, "Error encoding a frame: \n");
                return ;
            }
            pkt->stream_index = 0;
            pkt->duration=(1000/enc_ctx->time_base.den)*(j+1);
            pkt->pos=-1;

            j=0;
            av_interleaved_write_frame(ofmt_ctx, pkt);
        }

    }
    av_write_trailer(ofmt_ctx);


    avio_closep(&ofmt_ctx->pb);

    av_frame_free(&frame);
    av_packet_free(&pkt);
    av_dict_free(&opts);
    avcodec_free_context(&enc_ctx);
    avformat_close_input(&ofmt_ctx);
    sws_freeContext(sws_ctx);

    return ;
}
