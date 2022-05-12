//
// Created by Denis Shevchenko on 28.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>


#include "Ccmd.h"
#include "SOIL.h"
#include "CFFreader.h"
#include "CffmpegStreamer.h"
#include "SstreamData.h"

Ccmd::Ccmd(){
    clearPresImage();
    for(int i=0; i<6; i++){
        CFFreader cfFreader;
        FFreader.push_back(cfFreader);
    }
    mainImageData= (unsigned char*)malloc(1280*720*3*sizeof (unsigned char));
    for(int i=0;i<1280*720;i++){
        mainImageData[i*3+0]=0xff;
        mainImageData[i*3+0]=0x0f;
        mainImageData[i*3+0]=0xff;
    }

};
void Ccmd::notifyStreamStarted(std::string eventid, std::map<std::string, SstreamData*> *pStreamers){
    std::cout<< " notifyStreamStarted" << eventid<< std::endl;

};
void Ccmd::notifyStreamEnded(std::string eventid, std::map<std::string, SstreamData*> *pStreamers){
    std::cout<< " notifyStreamEnded"  << eventid << std::endl;

};


void Ccmd::startReadStream(std::string rtmpURL, int layerNumber ){

    FFreader[layerNumber].dt.width=layerNumber;
    FFreader[layerNumber].dt.layer=layerNumber;
    std::cout<< "start input " << rtmpURL<<" " << layerNumber<< std::endl;
    std::cout<< "start  " << (FFreader[layerNumber].dt.width) <<" " << layerNumber<< std::endl;
   // worker.work(rtmpURL);

    std::thread ffmpegThread(FFreader[layerNumber].work, rtmpURL, &FFreader[layerNumber].dt , &locker );
    ffmpegThread.detach();



   // ffmpegThread

}
void Ccmd::loadPresImage(std::string filepath, const std::string simageid){

   // auto start = std::chrono::system_clock::now();

    clearPresImage();

    std::lock_guard<std::mutex> lockGuard(locker);
    imageid=simageid;
    free(PresImagePixels);

    /* PresImagePixels =SOIL_load_image(filepath.c_str(),
                                  &PresImageWidth,
                                  &PresImageHeight,
                                  0,
                                  SOIL_LOAD_RGB);*/
    PresImageWidth=1280;
    PresImageHeight=720;
    PresImagePixels=(unsigned char *) malloc(PresImageWidth*PresImageHeight*3* sizeof(unsigned char ));
    std::cout<<"PresImagePixels"<<" "<< PresImageWidth << ""<< std::endl;
    int i=0;
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    do{
      //  std::cout<<i<<std::endl;

        PresImagePixels[i+0]= ((double)rand() / RAND_MAX)*254;;
        PresImagePixels[i+1]= ((double)rand() / RAND_MAX)*254;;
        PresImagePixels[i+2]= ((double)rand() / RAND_MAX)*254;;
        i=i+3;
    } while(i< PresImageWidth*PresImageHeight*3);
    std::cout<<"PresImagePixels complite "<<" "<< ((double)rand() / RAND_MAX)*16 << ""<< std::endl;
    std::remove(filepath.c_str());
    activeTextureId.clear();
    /*auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;*/

}
void Ccmd::clearPresImage(){
    std::lock_guard<std::mutex> lockGuard(locker);
    PresImageWidth=0;
    PresImageHeight=0;
    imageid="";
       // free(PresImagePixels);
    PresImagePixels;
       // PresImagePixels=nullptr;

};
int Ccmd::startStream(const std::string eventid,  std::map<std::string, SstreamData *> *pStreamers){
    _pStreamers = pStreamers;
    if(pStreamers->find(eventid)!=pStreamers->end()) {
        std::cout<<  "Error : straamer already created" <<std::endl;
        return  -1;
    }
    printf("startStream\n");
    std::thread streamThread(CffmpegStreamer::startStream, eventid, mainImageData,  (std::function<void(std::string, SstreamData*)>) notifyStreamStarted,  (std::function<void(std::string , SstreamData*)> )notifyStreamEnded, pStreamers);

    streamThread.join();
    SstreamData dt;
    dt.eventid=eventid;
    dt.thread=&streamThread;
    pStreamers->insert(std::pair<std::string, SstreamData *> ( eventid, &dt ));

    return 0;
}