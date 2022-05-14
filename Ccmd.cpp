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
#include <Magick++.h>


#include "CConfig.h"
#include "Ccmd.h"
#include "SOIL.h"
#include "CFFreader.h"
#include "CffmpegStreamer.h"
//#include "SstreamData.h" // TODO:for delete
#include "CEvent.h"


Ccmd::Ccmd() {
    clearPresImage();
};

void Ccmd::makeMainImage(std::string eventid, unsigned char *mainImageData, std::vector<unsigned char *> previewImageData,
                    std::mutex *locker, std::function<void(std::string eventid)> onStart,
                    std::function<void(std::string eventid)> onEnd) {
    onStart(eventid);
    int ww = CConfig::WIDTH / 4;
    int hh = CConfig::HEIGHT / 4;
    int memorySize = CConfig::WIDTH * CConfig::HEIGHT * 3 * sizeof(unsigned char);

    unsigned char *blankImage = (unsigned char *) malloc(memorySize);
    for (int i = 0; i < memorySize; i++) {
        blankImage[i] = 0xff;
    }
    Magick::InitializeMagick(nullptr);
    Magick::Image image;
    image.read(CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, blankImage);

    using namespace Magick;

    long long i = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        using namespace std::chrono_literals;

        i++;

        for(int i=0;i< /*previewImageData.size()*/1;i++)//TODO: uncomment
        {
            Magick::Image imageInput;
            imageInput.read(CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, previewImageData[i]);
            imageInput.resize( Magick::Geometry(ww, hh));
            if(i<4)
                image.composite(imageInput,ww*i, 0);
            else
                image.composite(imageInput,ww*3, (hh*(i-3)));

        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
    //    std::cout << "render image " << i << " sleep: "
               //   << (int) (std::chrono::milliseconds(1000 / FRAMERATE).count() - elapsed.count()) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds((int) ((1000 / CConfig::FRAMERATE) - elapsed.count())));

        locker->lock();
        image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, mainImageData);
        locker->unlock();


        start = std::chrono::high_resolution_clock::now();
    }
}

void Ccmd::notifyMakeMainImageStarted(std::string eventid) {
    std::cout << "notifyMakeMainImageStarted " << eventid << std::endl;
}

void Ccmd::notifyMakeMainImageEnded(std::string eventid) {

}

unsigned char *Ccmd::loadNotConnected(int input) {
    std::string fileName("/etc/mixerCore/images/notconnected");
    fileName.append(std::to_string(input + 1));
    fileName.append(".png");
    int h = CConfig::HEIGHT;
    int w = CConfig::WIDTH;
    Magick::InitializeMagick(nullptr);
    Magick::Image image;
    image.read(fileName.c_str());
    // image.crop( Magick::Geometry(0,0, WIDTH, HEIGHT) );
    unsigned char *pixels = (unsigned char *) malloc(CConfig::HEIGHT * CConfig::WIDTH * 3 * sizeof(unsigned char));
    image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, pixels);

    return pixels;

}

void Ccmd::notifyStreamStarted(std::string eventid) {

    std::cout << " notifyStreamStarted" << eventid << std::endl;

};

void Ccmd::notifyStreamEnded(std::string eventid) {
    std::cout << " notifyStreamEnded" << eventid << std::endl;


};


void Ccmd::startReadStream(std::string rtmpURL, int layerNumber) { // TODO: Delete!!!!

    FFreader[layerNumber].dt.width = layerNumber;
    FFreader[layerNumber].dt.layer = layerNumber;
    std::cout << "start input " << rtmpURL << " " << layerNumber << std::endl;
    std::cout << "start  " << (FFreader[layerNumber].dt.width) << " " << layerNumber << std::endl;
    // worker.work(rtmpURL);

    std::thread ffmpegThread(FFreader[layerNumber].work, rtmpURL, &FFreader[layerNumber].dt, &locker);
    ffmpegThread.detach();





    // ffmpegThread

}

void Ccmd::loadPresImage(std::string filepath, const std::string simageid) {

    // auto start = std::chrono::system_clock::now();

    clearPresImage();

    std::lock_guard<std::mutex> lockGuard(locker);
    imageid = simageid;
    free(PresImagePixels);

    /* PresImagePixels =SOIL_load_image(filepath.c_str(),
                                  &PresImageWidth,
                                  &PresImageHeight,
                                  0,
                                  SOIL_LOAD_RGB);*/
    PresImageWidth = 1280;
    PresImageHeight = 720;
    PresImagePixels = (unsigned char *) malloc(PresImageWidth * PresImageHeight * 3 * sizeof(unsigned char));
    std::cout << "PresImagePixels" << " " << PresImageWidth << "" << std::endl;
    int i = 0;
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    do {
        //  std::cout<<i<<std::endl;

        PresImagePixels[i + 0] = ((double) rand() / RAND_MAX) * 254;;
        PresImagePixels[i + 1] = ((double) rand() / RAND_MAX) * 254;;
        PresImagePixels[i + 2] = ((double) rand() / RAND_MAX) * 254;;
        i = i + 3;
    } while (i < PresImageWidth * PresImageHeight * 3);
    std::cout << "PresImagePixels complite " << " " << ((double) rand() / RAND_MAX) * 16 << "" << std::endl;
    std::remove(filepath.c_str());
    activeTextureId.clear();
    /*auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;*/

}

void Ccmd::clearPresImage() {
    std::lock_guard<std::mutex> lockGuard(locker);
    PresImageWidth = 0;
    PresImageHeight = 0;
    imageid = "";
    // free(PresImagePixels);
    PresImagePixels;
    // PresImagePixels=nullptr;

};

int Ccmd::startEvent(const std::string eventid) {


    /*   if(Ccmd::_Events.find(eventid)!=_Events.end())
      {
          CConfig::log("start Event:: already started", eventid);
          return 0;
      }
     CConfig::log("startEvent", eventid);

      CEvent *event=new CEvent(eventid);

      Ccmd::_Events.insert({eventid, event});

      int w = CConfig::WIDTH;
      int h = CConfig::HEIGHT;
      event->mainImageData = SOIL_load_image("/etc/mixerCore/images/pgmbg.png",
                                      &w,
                                      &h,
                                      0,
                                      SOIL_LOAD_RGB);*/

  /*  std::thread streamThread(CffmpegStreamer::startStream, eventid, event->mainImageData, & event->locker,
                             (std::function<void(std::string)>) notifyStreamStarted,
                             (std::function<void(std::string)>) notifyStreamEnded);
    streamThread.detach();


    event->thread = &streamThread;

    std::thread makeMainImageThread(Ccmd::makeMainImage, eventid, event->mainImageData, event->previewImageData, & event->locker,
                                    (std::function<void(std::string eventid)>) notifyMakeMainImageStarted,
                                    (std::function<void(std::string eventid)>) notifyMakeMainImageEnded);
    makeMainImageThread.detach();*/

    return 0;
}