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
    _eventid=eventid;
    stop= false;
    for(int i=0; i<CConfig::MAX_FACES; i++) {

        std::string fileName("/etc/mixerCore/images/notconnected");
        fileName.append(std::to_string(i + 1));
        fileName.append(".png");
        int h = CConfig::HEIGHT;
        int w = CConfig::WIDTH;
        Magick::InitializeMagick(nullptr);
        Magick::Image image;
        image.read(fileName.c_str());
        // image.crop( Magick::Geometry(0,0, WIDTH, HEIGHT) );
        unsigned char *pixels = (unsigned char *) malloc(CConfig::HEIGHT * CConfig::WIDTH * 3 * sizeof(unsigned char));
        image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, pixels);
        previewImageData.insert(pixels);
    }
}