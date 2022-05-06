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
void Ccmd::startReadStream(std::string rtmpURL, int layerNumber ){
    auto worker=FFreader[layerNumber];
    std::cout<< "start input " << rtmpURL<<" " << layerNumber<< std::endl;

}
void Ccmd::loadPresImage(std::string filepath, const std::string simageid){

   // auto start = std::chrono::system_clock::now();

    clearPresImage();

    std::lock_guard<std::mutex> lockGuard(locker);
    imageid=simageid;
    free(PresImagePixels);
    PresImagePixels =SOIL_load_image(filepath.c_str(),
                                 &PresImageWidth,
                                 &PresImageHeight,
                                 0,
                                 SOIL_LOAD_RGB);
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