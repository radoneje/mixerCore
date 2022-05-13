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

#include "settings.h"
#include "Ccmd.h"
#include "SOIL.h"
#include "CFFreader.h"
#include "CffmpegStreamer.h"
#include "SstreamData.h"
#include "SOIL.h"

Ccmd::Ccmd(){
    clearPresImage();
    /*
    for(int i=0; i<MAX_FACES; i++){
        CFFreader cfFreader;
        FFreader.push_back(cfFreader);
    }*/
    for(int i=0; i<MAX_FACES; i++){
        previewImageData.push_back(loadNotConnected(i));
    }
    previewImageData.push_back(nullptr);
   /* mainImageData= (unsigned char*)malloc(1280*720*3*sizeof (unsigned char));
    for(int i=0;i<1280*720;i++){
        mainImageData[(i*3)+0]=0x0f;
        mainImageData[(i*3)+1]=0x0f;
        mainImageData[(i*3)+2]=0xff;
    }*/

};

 void Ccmd::makeMainImage(std::string eventid, unsigned char * mainImageData, std::vector<unsigned char*> previewImageData,std::mutex *locker, std::function<void(std::string eventid)> onStart, std::function<void(std::string eventid)> onEnd ){
 onStart(eventid);
 int ww=WIDTH/4;
 int hh=HEIGHT/4;
 int memorySize=WIDTH * HEIGHT * 3 * sizeof(unsigned char);

 long long i=0;
 while(true) {
     i++;

     unsigned char *buf = (unsigned char *) malloc(memorySize);

     for (int y = 0; y < HEIGHT; y++)
         for (int x = 0; x < WIDTH; x++) {
          /*   buf[x+y+0]=mainImageData[x+y+0];
             buf[x+y+1]=mainImageData[x+y+1];
             buf[x+y+2]=mainImageData[x+y+2];*/
            // if(x<ww && y<hh){

             buf[(x*3)+(WIDTH*3*y)+0]=0xfe;
             buf[(x*3)+(WIDTH*3*y)+1]=0x00;
             buf[(x*3)+(WIDTH*3*y)+2]=0x00;

            // }

         }
     locker->lock();
    /// std::copy(buf, buf+memorySize, mainImageData);
     //free(mainImageData);
  //   mainImageData=buf;
     memcpy(mainImageData,buf, memorySize);
     locker->unlock();
    // locker->lock();
    // memcpy(mainImageData,buf, memorySize);


     std::cout<<"render image "<< i <<" "<< std::hex << (int)mainImageData[1] << " " << std::hex << (int) buf[1]<<endl;
     free(buf);
     std::this_thread::sleep_for(std::chrono::milliseconds(1000/FRAMERATE));
 }
}
void Ccmd::notifyMakeMainImageStarted(std::string eventid){
std::cout<<"notifyMakeMainImageStarted "<<eventid << std::endl;
  }
void Ccmd::notifyMakeMainImageEnded(std::string eventid){

  }

unsigned  char* Ccmd::loadNotConnected(int input){
    std::string fileName("/etc/mixerCore/images/notconnected");
    fileName.append(std::to_string(input+1));
    fileName.append(".png");
    int h=HEIGHT;
    int w=WIDTH;
    return  SOIL_load_image(fileName.c_str(),
                    &h,
                    &w,
                    0,
                    SOIL_LOAD_RGB);
}
void Ccmd::notifyStreamStarted(std::string eventid){

    std::cout<< " notifyStreamStarted" << eventid<< std::endl;

};
void Ccmd::notifyStreamEnded(std::string eventid){
    std::cout<< " notifyStreamEnded"  << eventid << std::endl;


};


void Ccmd::startReadStream(std::string rtmpURL, int layerNumber ){ // TODO: Delete!!!!

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
int Ccmd::startStream(const std::string eventid){

       int w=WIDTH;
       int h=HEIGHT;
     mainImageData =SOIL_load_image("/etc/mixerCore/images/pgmbg.png",
                        &w,
                        &h,
                        0,
                        SOIL_LOAD_RGB);


    printf("startStream\n");

    std::thread streamThread(CffmpegStreamer::startStream, eventid, mainImageData ,&locker, (std::function<void(std::string)>) notifyStreamStarted,  (std::function<void(std::string)>) notifyStreamEnded);
    streamThread.detach();

    SstreamData dt;
    dt.eventid=eventid;
    dt.thread=&streamThread;


    std::thread makeMainImageThread(Ccmd::makeMainImage, eventid, mainImageData, previewImageData, &locker, (std::function<void(std::string eventid)>) notifyMakeMainImageStarted, (std::function<void(std::string eventid)>) notifyMakeMainImageEnded);
    makeMainImageThread.detach();

    return 0;
}