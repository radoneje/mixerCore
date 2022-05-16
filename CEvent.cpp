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
    _eventid=eventid;
    stop= false;
    int imageSize=CConfig::WIDTH*CConfig::HEIGHT*3*sizeof(unsigned char);
    int previewImageSize=(CConfig::WIDTH/4)*(CConfig::HEIGHT/4)*3*sizeof(unsigned char);
    mainImageData=(unsigned char*)malloc(CConfig::WIDTH*CConfig::HEIGHT*3*sizeof(unsigned char));

    for(int i=0; i<CConfig::MAX_FACES; i++) {
        std::string fileName("/etc/mixerCore/images/notconnected");
        fileName.append(std::to_string(i + 1));
        fileName.append(".png");
        int h = CConfig::HEIGHT;
        int w = CConfig::WIDTH;

        SImageData imageDataItem;
        imageDataItem.fullImageData= (unsigned char*)malloc(imageSize);
        imageDataItem.previewImageData=(unsigned char*)malloc(previewImageSize);
        imageDataItem.itemid="";

        Magick::InitializeMagick(nullptr);
        Magick::Image image;
        image.read(fileName.c_str());

        image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, imageDataItem.fullImageData);
        image.resize(Magick::Geometry(CConfig::WIDTH/4,CConfig::HEIGHT/4));
        image.write(0, 0, CConfig::WIDTH/4, CConfig::HEIGHT/4, "RGB", MagickLib::CharPixel, imageDataItem.previewImageData);

        imageData.push_back(imageDataItem);
    }
    SImageData imageDataItem;
    imageDataItem.fullImageData= (unsigned char*)malloc(imageSize);

    for(int i=0;i<imageSize;i++)
        imageDataItem.fullImageData[i]=0x88;
    imageDataItem.previewImageData=(unsigned char*)malloc(previewImageSize);

    for(int i=0;i<previewImageSize;i++)
        imageDataItem.previewImageData[i]=0x66;

    imageDataItem.itemid="";
    imageData.push_back(imageDataItem);
}

void CEvent::showPres(unsigned char * data, std::string itemid){
    try {
   /*     Magick::Image image;
        image.read(CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel,
                   data);
        image.resize(Magick::Geometry(CConfig::WIDTH * 0.75, CConfig::HEIGHT * 0.75));
        locker.lock();
        image.write(0, 0, CConfig::WIDTH * 0.75, CConfig::HEIGHT * 0.75, "RGB", MagickLib::CharPixel,
                    imageData.back().fullImageData);
        image.resize(Magick::Geometry(CConfig::WIDTH * 0.25, CConfig::HEIGHT * 0.25));
        image.write(0, 0, CConfig::WIDTH / 4, CConfig::HEIGHT / 4, "RGB", MagickLib::CharPixel,
                    imageData.back().previewImageData);*/
        locker.lock();
        activeInputs.clear();
        activeInputs.push_back(CConfig::MAX_FACES);
        imageData.back().itemid = itemid;
        locker.unlock();
    }
    catch(...)
    {
        CConfig::error("Error cmd::showPres");
    };
};