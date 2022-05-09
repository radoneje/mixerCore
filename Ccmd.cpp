//
// Created by Denis Shevchenko on 28.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>


#include "Ccmd.h"
#include "SOIL.h"
#include "CFFreader.h"

Ccmd::Ccmd(){
    clearPresImage();
    for(int i=0; i<6; i++){
        CFFreader cfFreader;
        FFreader.push_back(cfFreader);
    }
};
void Ccmd::notifyStreamStarted(int layerNumber){
    std::cout<< " notifyStreamStarted" << layerNumber<< std::endl;
};
void Ccmd::notifyStreamEnded(int layerNumber){
    std::cout<< " notifyStreamEnded"  << layerNumber << std::endl;
};


void Ccmd::startReadStream(std::string rtmpURL, int layerNumber, vFunctionCall  notifyStreamStarted, vFunctionCall notifyStreamEnded ){

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
    do{
        std::cout<<i<<std::endl;
        PresImagePixels[i+0]=0xff;
        PresImagePixels[i+1]=0x00;
        PresImagePixels[i+2]=0x00;
        i=i+3;
    }while(i< PresImageWidth*PresImageHeight*3);

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