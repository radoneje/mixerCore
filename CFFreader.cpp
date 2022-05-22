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
#include "CConfig.h"


CFFreader::CFFreader(){
    dt.width=0;
    dt.height=0;
    dt.frameNumber=-1;
    dt.numChannels=0;
    dt.layer=-1;
    dt.pixels=  (unsigned char *) malloc(1 * sizeof(int));


};

 long CFFreader::nowTime() {
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int CFFreader::work(const std::string url, int inputNum, CEvent  *pEvent){//, Data &pData){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    AVFormatContext *ctx_format = nullptr;
    AVCodecContext *ctx_codec = nullptr;
    AVCodecContext *ctx_aud_codec = nullptr;
  //  AVCodec *codec = nullptr;
    AVCodec *aud_codec = nullptr;

    int stream_idx=-1;
    int audio_idx=-1;
    const char *fin =url.c_str();//"/tmp/vcbr.mp4";// url.c_str();;
    AVStream *vid_stream = nullptr;
    AVStream *aud_stream = nullptr;
    AVPacket *pkt = av_packet_alloc();
    int ret;



   

    if (int ret = avformat_open_input(&ctx_format, fin, nullptr, nullptr) != 0) {
        std::cout <<"ERROR avformat  " << 1 << std::endl;
        return 0;
    }


    ctx_format->probesize=500000;
   //ctx_format->max_analyze_duration=32000;

    std::cout <<"probesize  " << ctx_format->probesize << " max_analyze_duration "<<ctx_format->max_analyze_duration << std::endl;

    int avformatRez=avformat_find_stream_info(ctx_format, nullptr);
        if (avformatRez < 0) {
             ctx_format->probesize=5000000;

            avformatRez=avformat_find_stream_info(ctx_format, nullptr);
            if(avformatRez<0){
            std::cout << "ERROR avformat  " << 2 << std::endl;
            return 0; // Couldn't find stream information
            }
        }
    std::cout <<"in Worker 4"<<  CConfig::WIDTH << " " <<url << std::endl;
    av_dump_format(ctx_format, 0, fin, false);

    for (int i = 0; i < ctx_format->nb_streams; i++) {
        if (ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && stream_idx<0) {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            //break;
        }
    }

    if (vid_stream == nullptr) {
        std::cout<<"ERROR avformat VIDEO " << 4 << std::endl;
        return 0;
    }

    const AVCodec  *codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
    if (!codec) {
        std::cout << "ERROR codec not found" << std::endl;
        return 0;
    }

    ctx_codec = avcodec_alloc_context3(codec);


    if (avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar) < 0)
        std::cout << 512 << ctx_codec->pix_fmt << std::endl;
   // ctx_codec->hwaccel = ff_find_hwaccel(ctx_codec->codec->id, ctx_codec->pix_fmt);

    std::cout << 514 << ctx_codec->hwaccel << std::endl;


    if (avcodec_open2(ctx_codec, codec, nullptr) < 0) {
        std::cout << "ERROR avcodec_open2"<< std::endl;
        return 0;
    }

    ;

    
    std::cout << 94 << std::endl;


    // задержка на частоту кадров
    int frameDur=0;
    if(vid_stream->avg_frame_rate.num>0)
     frameDur = (vid_stream->avg_frame_rate.den * 1000) / vid_stream->avg_frame_rate.num;
    long lastFrameTime = 0;
    std::cout<< "frameDur" <<frameDur  << std::endl;
   // finalFrameData_lock.lock();
    {
       // std::lock_guard<std::mutex> lockGuard(pEvent->locker);

    }
   // finalFrameData_lock.unlock();

    int ii = 0;
    std::cout<< "av_lib ready to the read" << std::endl;
    std::cout << "Time difference = " <<( std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count())/1000 << "[ms]" << std::endl;

    //////////

    AVFrame *frame = av_frame_alloc();
    AVFrame *pRGBFrame = av_frame_alloc();
    AVFrame *pRGBFrame_preview = av_frame_alloc();

    pRGBFrame->format = AV_PIX_FMT_RGB24;
    pRGBFrame->width = CConfig::WIDTH*0.75;
    pRGBFrame->height = CConfig::HEIGHT*0.75;

    pRGBFrame_preview->format = AV_PIX_FMT_RGB24;
    pRGBFrame_preview->width = CConfig::WIDTH*0.25;
    pRGBFrame_preview->height = CConfig::HEIGHT*0.25;

    int sts = av_frame_get_buffer(pRGBFrame, 0);
    if (sts < 0) {
        std::cout <<"ERROR av_frame_get_buffer" << 4444 << std::endl;
        return 0;  //Error!
    }
    int sts_preview=av_frame_get_buffer(pRGBFrame_preview, 0);
    if (sts_preview < 0) {
        std::cout <<"ERROR av_frame_get_buffer" << 4444 << std::endl;
        return 0;  //Error!
    }
    if(!pEvent->onInputStart(inputNum))
        return 0;
    while (av_read_frame(ctx_format, pkt) >= 0) {
        //

        if (pkt->stream_index == stream_idx) {
            //Allocate frame for storing image converted to RGB.
            ////////////////////////////////////////////////////////////////////////////



            int ret = avcodec_send_packet(ctx_codec, pkt);
            if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                std::cout << "avcodec_send_packet: " << ret << " " << AVERROR(EAGAIN) << " " << AVERROR_EOF<<std::endl;

                av_frame_free(&pRGBFrame);
                av_frame_free(&frame);
                av_frame_free(&pRGBFrame_preview);
                break;
            }
            while (ret >= 0) {


                ret = avcodec_receive_frame(ctx_codec, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    //std::cout << "avcodec_receive_frame: " << ret << std::endl;

                    break;
                }
                ii++;
                //std::cout << " " <<frame->width  <<" "<< frame->height<< std::endl;
               // int64_t pts = av_rescale(frame->pts, 1000000, AV_TIME_BASE);
               // int64_t now = av_gettime_relative();//- frame->start;
                 int  dstStride= (int)(CConfig::WIDTH*0.75*3);
               
               float coof=(float)CConfig::HEIGHT/(float)CConfig::WIDTH;
               //std::cout<<coof<< " "<<CConfig::WIDTH*coof<<  std::endl;
                   
                try {
                    struct SwsContext *sws_ctx = sws_getContext(
                             frame->width,
                             frame->height,
                             ctx_codec->pix_fmt,
                             CConfig::WIDTH*0.75,
                             CConfig::HEIGHT*0.75,
                             AV_PIX_FMT_RGB24,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);
                struct SwsContext *sws_ctx_preview = sws_getContext(
                             frame->width,
                             frame->height,
                             ctx_codec->pix_fmt,
                             CConfig::WIDTH*0.25,
                             CConfig::HEIGHT*0.25,
                             AV_PIX_FMT_RGB24,
                             SWS_BICUBIC,
                             NULL,
                             NULL,
                             NULL);
                            

                    sts = sws_scale(sws_ctx,                //struct SwsContext* c,
                                    frame->data,            //const uint8_t* const srcSlice[],
                                    frame->linesize,        //const int srcStride[],
                                    0,                      //int srcSliceY,
                                    frame->height,          //int srcSliceH,
                                    pRGBFrame->data,        //uint8_t* const dst[],
                                    &dstStride);   //const int dstStride[]);
                   // std::cout << ">height " <<frame->linesize  <<" "<< frame->height<< std::endl;
                   if (sts != CConfig::HEIGHT *0.75) {
                        std::cout << "sts != frame->height " <<sts  <<" "<< frame->height<< std::endl;
                       // return 0;  //Error!
                    }

                    dstStride= (int)(CConfig::WIDTH*0.25*3);

                    sts_preview = sws_scale(sws_ctx_preview,                //struct SwsContext* c,
                                    frame->data,            //const uint8_t* const srcSlice[],
                                    frame->linesize,        //const int srcStride[],
                                    0,                      //int srcSliceY,
                                    frame->height,          //int srcSliceH,
                                    pRGBFrame_preview->data,        //uint8_t* const dst[],
                                    &dstStride);
                    if (sts_preview != CConfig::HEIGHT *0.25) {
                        std::cout << "sts != sts_preview_frame->height " <<sts_preview  <<" "<< frame->height*0.25<< std::endl;
                        //return 0;  //Error!
                    }

                     

                    // задержка на частоту кадров
                    long thisFrameTime = lastFrameTime + frameDur;
                    if (thisFrameTime > nowTime()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(thisFrameTime - nowTime()));
                    }
                    lastFrameTime = nowTime();
                    // std::cout<< "av_read_frame: "   << ii << std::endl;
                    {
                      //  std::lock_guard<std::mutex> lockGuard(pEvent->locker);

                        std::size_t size =
                                pRGBFrame->width * pRGBFrame->height * (pRGBFrame->linesize[0] / pRGBFrame->width) *
                                sizeof(unsigned char);
                        /*   free(pEvent->imageData[inputNum].fullImageData);

                           pEvent->imageData[inputNum].fullImageData= (unsigned char *) malloc(size);
                           */
                       memcpy(pEvent->imageData[inputNum].fullImageData, pRGBFrame->data[0], size);
                         size =
                                pRGBFrame_preview->width * pRGBFrame_preview->height * (pRGBFrame_preview->linesize[0] / pRGBFrame_preview->width) *
                                sizeof(unsigned char);
                        memcpy(pEvent->imageData[inputNum].previewImageData, pRGBFrame_preview->data[0], size);
                        pEvent->imageData[inputNum].frameNumber = ctx_codec->frame_number;


                        

                    }
                     sws_freeContext(sws_ctx);
                        sws_freeContext(sws_ctx_preview);
                } catch (...){
                    std::cout<< "Error in ffmpeg processing" << std::endl;
                }

            }

           /* av_frame_free(&frame);
            av_frame_free(&pRGBFrame);
            av_frame_free(&pRGBFrame_preview);*/

        }
    }
    av_frame_free(&frame);
    av_frame_free(&pRGBFrame);
    av_frame_free(&pRGBFrame_preview);
    ///////////
    {
        std::lock_guard<std::mutex> lockGuard(pEvent->locker);
      //  if(pData->width>720 && pData->width<1921)
       //     free(pData->pixels);
      /*  pData->width = 0;
        pData->height = 0;
        pData->frameNumber = -1;*/
    }
    pEvent->onInputEnd(inputNum);
    std::this_thread::sleep_for(std::chrono::milliseconds(frameDur));

    av_packet_unref(pkt);
  
    avcodec_free_context(&ctx_codec);
    avformat_close_input(&ctx_format);

    return 0;
}

