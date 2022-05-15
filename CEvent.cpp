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
    for(int i=0; i<CConfig::MAX_FACES; i++) {
        std::string fileName("/etc/mixerCore/images/notconnected");
        fileName.append(std::to_string(i + 1));
        fileName.append(".png");
        int h = CConfig::HEIGHT;
        int w = CConfig::WIDTH;
        SImageData imageDataItem;
        Magick::InitializeMagick(nullptr);
        Magick::Image image;
        image.read(fileName.c_str());

        image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, imageDataItem.fullImageData);
        image.resize(Magick::Geometry(CConfig::WIDTH/4,CConfig::HEIGHT/4));
        image.write(0, 0, CConfig::WIDTH/4, CConfig::HEIGHT/4, "RGB", MagickLib::CharPixel, imageDataItem.previewImageData);

        imageData.push_back(imageDataItem);
    }
    SImageData imageDataItem;
    imageData.push_back(imageDataItem);
}