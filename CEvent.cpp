//
// Created by Denis Shevchenko on 14.05.2022.
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
#include <Magick++.h>

#include "CEvent.h"
#include "CConfig.h"

CEvent::CEvent(std::string eventid) {
    startTime = std::chrono::system_clock::now().time_since_epoch().count();
    _eventid = eventid;
    stop = false;
    int imageSize = (CConfig::WIDTH*0.75) * (CConfig::HEIGHT*0.75) * 3 * sizeof(unsigned char);
    int previewImageSize = (CConfig::WIDTH / 4) * (CConfig::HEIGHT / 4) * 3 * sizeof(unsigned char);
    mainImageData = (unsigned char *) malloc(CConfig::WIDTH * CConfig::HEIGHT * 3 * sizeof(unsigned char));

    const auto p1 = std::chrono::system_clock::now();
    for (int i = 0; i < CConfig::MAX_FACES; i++) {
        
        
        SInputData * input = new SInputData;
        input->isActive=false;
        input->number=i;
        input->spkid="",
        input->time_start=p1.time_since_epoch().count();

        inputs.insert({i, input});

        imageData.push_back(loadDefaultInput(i));
    }
    SImageData imageDataItem;
    imageDataItem.fullImageData = (unsigned char *) malloc(imageSize);

    for (int i = 0; i < imageSize; i++)
        imageDataItem.fullImageData[i] = 0x88;
    imageDataItem.previewImageData = (unsigned char *) malloc(previewImageSize);

    for (int i = 0; i < previewImageSize; i++)
        imageDataItem.previewImageData[i] = 0x66;

    imageDataItem.itemid = "";
    imageData.push_back(imageDataItem);
}
CEvent::SImageData CEvent::loadDefaultInput(int inputNo){
    int imageSize = (CConfig::WIDTH*0.75) * (CConfig::HEIGHT*0.75) * 3 * sizeof(unsigned char);
    int previewImageSize = (CConfig::WIDTH / 4) * (CConfig::HEIGHT / 4) * 3 * sizeof(unsigned char);


    std::string fileName("/etc/mixerCore/images/notconnected");
    fileName.append(std::to_string(inputNo + 1));
    fileName.append(".png");
    int h = CConfig::HEIGHT;
    int w = CConfig::WIDTH;

    SImageData imageDataItem;
    imageDataItem.fullImageData = (unsigned char *) malloc(imageSize);
    imageDataItem.previewImageData = (unsigned char *) malloc(previewImageSize);
    imageDataItem.itemid = "";

    Magick::InitializeMagick(nullptr);
    Magick::Image image;
    image.read(fileName.c_str());
    image.resize(Magick::Geometry(CConfig::WIDTH *0.75, CConfig::HEIGHT *0.75));
    image.write(0, 0, CConfig::WIDTH*0.75, CConfig::HEIGHT*0.75, "RGB", MagickLib::CharPixel, imageDataItem.fullImageData);
    image.resize(Magick::Geometry(CConfig::WIDTH / 4, CConfig::HEIGHT / 4));
    image.write(0, 0, CConfig::WIDTH / 4, CConfig::HEIGHT / 4, "RGB", MagickLib::CharPixel,
                imageDataItem.previewImageData);
    return imageDataItem;
};

void CEvent::showPres(unsigned char *data, std::string itemid) {
    try {

        Magick::Image image;

        image.read(CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel,
                   data);
        image.resize(Magick::Geometry(CConfig::WIDTH * 0.75, CConfig::HEIGHT * 0.75));
        locker.lock();
            image.write(0, 0, CConfig::WIDTH * 0.75, CConfig::HEIGHT * 0.75, "RGB", MagickLib::CharPixel,
                        imageData.back().fullImageData);
            image.resize(Magick::Geometry(CConfig::WIDTH * 0.25, CConfig::HEIGHT * 0.25));
            image.write(0, 0, CConfig::WIDTH / 4, CConfig::HEIGHT / 4, "RGB", MagickLib::CharPixel,
                        imageData.back().previewImageData);

            activeInputs.clear();
            activeInputs.push_back(CConfig::MAX_FACES);
            imageData.back().itemid = itemid;
        locker.unlock();
    }
    catch (...) {
        CConfig::error("Error cmd::showPres");
    };
};
bool CEvent::onInputStart(int inputNo){
    CConfig::log("Input Start", _eventid, inputNo);

    if(inputs.at(inputNo)->isActive==true) {
        CConfig::error("input already started");
        return false;
    }
    CEvent::SInputData *inputData= new SInputData();

    
    locker.lock();
    auto input=inputs.at(inputNo); //.insert({inputNo,inputData });
        input->isActive=true;
        input->time_start=std::chrono::system_clock::now().time_since_epoch().count();
    locker.unlock();
   

    auto s=std::to_string(inputNo);
    CConfig::notifyControl("inputStart",_eventid,&s,&input->spkid);
     return  true;
  

}
void CEvent::onInputEnd(int inputNo){
    CConfig::log("Input Stop", _eventid, inputNo);
    if(inputs.find(inputNo)==inputs.end()) {
        CConfig::error("input not present");
        return;
    }
    locker.lock();
    inputs.at(inputNo)->isActive=false;
    imageData[inputNo]=loadDefaultInput(inputNo);
    locker.unlock();

    auto s=std::to_string(inputNo);
    CConfig::notifyControl("inputStop",_eventid,&s,&inputs.at(inputNo)->spkid);
}
bool CEvent::onVideoLoaded(std::string fileid){
    CConfig::log("VIDEO LOADED", fileid);

    CEvent::SVideoFileData item;
    item.fileid= fileid,
    item.isPaused=true;
    item.isReady=false;

   locker.lock();
    if(videoFileReaders.find(fileid)!=videoFileReaders.end()){
        locker.unlock();
        return false;
    }
    videoFileReaders.insert({ fileid, &item}); 
    locker.unlock();
    return true;
};
void  CEvent::onVideoEnd(std::string fileid){
CConfig::log("VIDEO END", fileid);
    locker.lock();
    if(videoFileReaders.find(fileid)==videoFileReaders.end()){
        locker.unlock();
        return ;
    }
    videoFileReaders.erase(fileid);
    locker.unlock();
}