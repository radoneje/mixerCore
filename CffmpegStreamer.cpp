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

    std::cout<<" packet: " <<pkt->duration<< " "<< pkt->dts<<std::endl;
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




    AVStream video_stream = { 0 };
    const AVOutputFormat *fmt;

    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *c= NULL;
    AVFormatContext *pFormatCtx = NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVFormatContext *octx = NULL;

    AVDictionary *opt = NULL;
   // av_dict_set(&opt, argv[i]+1, argv[i+1], 0);

    AVPacket *pkt;

    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    struct SwsContext *sws_ctx = NULL;
// Выходной поток
    // Создаем контекст выходного потока

    ret = avformat_alloc_output_context2(&octx, 0, "flv", "rtmp://wowza01.onevent.online/live/t");
    if (!octx)
    {
        std::cout<<"ERROR avformat_alloc_output_context2"<<std::endl;
        retrurn;
    }
    fmt = octx->oformat;
    avformat_write_header(pFormatCtx, &opt);


    filename = "/var/www/mixerControl/public/1.mp4";
    codec_name = "libx264";
    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }





    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);
    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 1280;
    c->height = 720;
    /* frames per second */
    c->time_base = (AVRational){1, 25};
    c->framerate = (AVRational){25, 1};
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);
    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n");//, av_err2str(ret));
        exit(1);
    }
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;
    std::cout<< "frame lainesixe: "<< frame->linesize<<std::endl;
    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
    sws_ctx = sws_getContext(c->width,
                             c->height,
                             AV_PIX_FMT_RGB24,
                             c->width,
                             c->height,
                             AV_PIX_FMT_YUV420P,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);

    std::cout<<"fmt->video_codec: "<<fmt->video_codec<<std::endl;
  //  add_stream(&video_stream, octx, codec, fmt->video_codec);

    long long startTime = av_gettime();
    /* encode 1 second of video */
    for (i = 0; i < 100; i++) {
        fflush(stdout);
        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);
        /* prepare a dummy image */
        /* Y */
      /*  for (y = 0; y < c->height; y++) {
            for (x = 0; x < c->width; x++) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }
        // Cb and Cr
        for (y = 0; y < c->height/2; y++) {
            for (x = 0; x < c->width/2; x++) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }*/

        ret = sws_scale(sws_ctx,                //struct SwsContext* c,
                        &image,            //const uint8_t* const srcSlice[],
                        frame->linesize,        //const int srcStride[],
                        0,                      //int srcSliceY,
                        frame->height,          //int srcSliceH,
                        frame->data,        //uint8_t* const dst[],
                        frame->linesize);   //const int dstStride[]);

        frame->pts = i;
        long long now = av_gettime() - startTime;
        long long dts = 0;
        dts = (frame->pts) * ( r2d(c->time_base )*1000*1000);
        std::cout<<dts << " " << now<<std::endl;
        if (dts > now) {
            std::cout<<dts - now << " sleep" <<std::endl;
            av_usleep(dts - now);
        }
        ret = avcodec_send_frame(c, frame);
        if (ret < 0) {
            fprintf(stderr, "Error sending a frame to the encoder: %s\n"
                   );
            exit(1);
        }
        while (ret >= 0) {
            ret = avcodec_receive_packet(c, pkt);

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
                fprintf(stderr, "Error encoding a frame: %s\n");
                exit(1);
            }
            std::cout<<"avcodec_receive_packet "<< ret <<std::endl;
            /* rescale output packet timestamp values from codec to stream timebase */
           // av_packet_rescale_ts(pkt, c->time_base, st->time_base);
           // pkt->stream_index = st->index;

            /* Write the compressed frame to the media file. */
            log_packet(pFormatCtx, pkt);
            ret = av_interleaved_write_frame(octx, pkt);
            std::cout<<"av_interleaved_write_frame"<<std::endl;
        }
       /* ret = av_interleaved_write_frame(octx, &pkt);
        if (ret<0)
        {
            return ff_Error(ret);
        }*/

        /* encode the image */
       // encode(c, frame, pkt, f);
    }
    /* flush the encoder */
   // encode(c, NULL, pkt, f);
    /* add sequence end code to have a real MPEG file */
   // fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    return ;
}
