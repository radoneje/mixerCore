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


using namespace std;

 void CffmpegStreamer::log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
   // AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

   // std::cout<<" packet: " <<pkt->duration<< " "<< pkt->dts<<std::endl;
  /*  printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);*/
}

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
void CffmpegStreamer::encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    int ret;
    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }

}
void CffmpegStreamer::startStream(const std::string eventid, unsigned char * image,  std::function<void(std::string, streamersDataType *)> startCallback,   std::function<void(std::string, streamersDataType *)> EndCallback, std::map<std::string, SstreamData *> *pStreamers){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    AVStream *out_stream;
    AVFormatContext * ofmt_ctx = NULL;
    const AVCodec *encoder;
    AVCodecContext *enc_ctx;
    AVPacket *pkt;
    AVFrame *frame;
    SwsContext *sws_ctx = NULL;
    AVDictionary *opts = nullptr;

    std::string outUrl="rtmp://wowza01.onevent.online/live/mixerCore3";
    std::string codec_name = "libx264";

    int ret;

    avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", outUrl.c_str());
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return ;
    }



    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream) {
        av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
        return ;
    }
    encoder = avcodec_find_encoder_by_name(codec_name.c_str());
    if (!encoder) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name.c_str());
        return ;
    }
    enc_ctx = avcodec_alloc_context3(encoder);
    /* put sample parameters */
    enc_ctx->bit_rate = 1024*1024*1.5;
    /* resolution must be a multiple of two */
    enc_ctx->width = 1280;
    enc_ctx->height = 720;
    /* frames per second */
    enc_ctx->time_base = (AVRational){1, 30};
    enc_ctx->framerate = (AVRational){30, 1};
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    enc_ctx->gop_size = 10;
    enc_ctx->max_b_frames = 1;
    enc_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    if (encoder->id == AV_CODEC_ID_H264)
        av_opt_set(enc_ctx->priv_data, "preset", "slow", 0);
    /* open it */


    av_dict_set(&opts, "preset", "superfast", 0);
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

   // stream_ctx[i].enc_ctx = enc_ctx;
    ret=avio_open2(&ofmt_ctx->pb, outUrl.c_str(), AVIO_FLAG_WRITE , NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error avio_open\n");
        return;
    }
    ret = avformat_write_header(ofmt_ctx, 0);
    if (ret < 0) {
        fprintf(stderr, "Error avformat_write_header\n");
        return;
    }
    //////////////////

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
    long long i=0;
        while(true) {
            i++;
        fflush(stdout);
        ret = av_frame_make_writable(frame);
        /* prepare a dummy image */
        /* Y */
         for (int y = 0; y < enc_ctx->height; y++) {
              for (int x = 0; x < enc_ctx->width; x++) {
                  frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
              }
          }
          // Cb and Cr
          for (int y = 0; y < enc_ctx->height/2; y++) {
              for (int x = 0; x < enc_ctx->width/2; x++) {
                  frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                  frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
              }
          }
        frame->pts = i;
        long long now = av_gettime() - startTime;
        long long dts = 0;
        dts = (frame->pts) * ( r2d(enc_ctx->time_base )*1000*1000);
       // std::cout<<dts << " " << now<<std::endl;
        if (dts > now) {
          //  std::cout<<dts - now << " sleep" <<std::endl;
            av_usleep(dts - now);
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
            else if (ret < 0) {
                fprintf(stderr, "Error encoding a frame: \n");
                return ;
            }
          //  std::cout<<"avcodec_receive_packet " <<std::endl;
            av_packet_rescale_ts(pkt,
                                 enc_ctx->time_base,
                                 ofmt_ctx->streams[0]->time_base);

            log_packet(ofmt_ctx, pkt);
            ret = av_interleaved_write_frame(ofmt_ctx, pkt);
            std::cout<<"av_interleaved_write_frame "<< i <<std::endl;
        }


    }
    avcodec_free_context(&enc_ctx);
    av_frame_free(&frame);
    av_packet_free(&pkt);


    return ;
}
