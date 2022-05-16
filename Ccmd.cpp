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
#include "CEvent.h"


Ccmd::Ccmd() {
    clearPresImage();
};

std::map<std::string, CEvent *> Ccmd::_Events;
std::mutex Ccmd::_locker;

void Ccmd::makeMainImage(std::string eventid,
                         CEvent *pEvent, std::function<void(std::string eventid)> onStart,
                         std::function<void(std::string eventid)> onEnd) {

    //  try {
    onStart(eventid);
    int ww = CConfig::WIDTH / 4;
    int hh = CConfig::HEIGHT / 4;
    int memorySize = CConfig::WIDTH * CConfig::HEIGHT * 3 * sizeof(unsigned char);

    unsigned char *blankImage = (unsigned char *) malloc(memorySize);
    for (int i = 0; i < memorySize; i++) {
        blankImage[i] = 0xff;
        pEvent->mainImageData[i] = 0xff;
    }

    Magick::InitializeMagick(nullptr);
    Magick::Image image;

    image.read(CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, blankImage);
    // free(blankImage);
    // using namespace Magick;


    long long i = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (true && !pEvent->stop) {
        using namespace std::chrono_literals;
        i++;
        pEvent->locker.lock();
      //  std::cout<<"before "<< i <<std::endl;
        for (int y = 0; y < CConfig::HEIGHT; y++)
            for (int x = 0; x < CConfig::WIDTH; x++) {
                /// находим PGM
                if(x<ww*3 && y>hh) {
                    //заполняем PGM
                    int pgmX=x; // координаты PGM
                    int pgmY=y-hh;
                    ///////// генерация презы
                   // std::cout<< "before if "<<std::endl;
                      if(pEvent->activeInputs.size()==1 && pEvent->activeInputs[0]==CConfig::MAX_FACES){
                         // std::cout<< "inside if "<<std::endl;
                          //std::cout<< pEvent->imageData.size()<< "size; " << pEvent->activeInputs[0] <<std::endl;
                          pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 0] =
                                  pEvent->imageData[pEvent->activeInputs[0]].fullImageData[(int)((pgmX + (pgmY * CConfig::WIDTH*0.75)) * 3) + 0];
                          pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 1] =
                                  pEvent->imageData[pEvent->activeInputs[0]].fullImageData[(int)((pgmX + (pgmY * CConfig::WIDTH*0.75)) * 3) + 1];
                          pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 2] =
                                  pEvent->imageData[pEvent->activeInputs[0]].fullImageData[(int)((pgmX + (pgmY * CConfig::WIDTH*0.75)) * 3) + 2];

                      }
                    else{ // пустой PGM
                        pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 0] = 0x00;
                        pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 1] = 0xf0;
                        pEvent->mainImageData[((x + (y * CConfig::WIDTH)) * 3) + 2] = 0x00;
                    }
                }
            }
        pEvent->locker.unlock();
        ////////генерация превьюшек
        for (int i = 0; i < CConfig::MAX_FACES; i++)//TODO: uncomment
        {
            //TODO: previewImageData-> заполнить и взять
            /*    Magick::Image imageInput;
                pEvent->locker.lock();
                imageInput.read(CConfig::WIDTH / 4, CConfig::HEIGHT / 4, "RGB", MagickLib::CharPixel,
                                pEvent->imageData[i].previewImageData);
                pEvent->locker.unlock();
                if (i < 4)
                    image.composite(imageInput, ww * i, 0);
                else
                    image.composite(imageInput, ww * 3, (hh * (i - 3)));*/

        }

        ///////// генерация презы
        /*  if(pEvent->activeInputs.size()==1 && pEvent->activeInputs[0]==CConfig::MAX_FACES){
              Magick::Image imageInput;
              pEvent->locker.lock();
              imageInput.read(CConfig::WIDTH *0.75, CConfig::HEIGHT *0.75, "RGB", MagickLib::CharPixel,
                              pEvent->imageData[1].fullImageData);
              pEvent->locker.unlock();
              image.composite(imageInput, 0 ,hh);
          }*/

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
       // std::cout << "sleep "
       //           << std::chrono::milliseconds((int) ((1000 / CConfig::FRAMERATE) - elapsed.count())).count()
        //          << std::endl;
        std::this_thread::sleep_for(
                std::chrono::milliseconds((int) ((1000 / CConfig::FRAMERATE) - elapsed.count())));


        start = std::chrono::high_resolution_clock::now();
    }


//  } catch (...) { CConfig::error("ERROR IN makeMainImage"); }
    onEnd(eventid);
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

void Ccmd::_stopEvent(std::string eventid) {
    std::lock_guard<std::mutex> lock(_locker);
    if (_Events.find(eventid) == _Events.end())
        return;
    _Events[eventid]->stop = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    _Events.erase(eventid);
}

void Ccmd::notifyStreamStarted(std::string eventid) {
    CConfig::log("Main stream loop started", eventid);
};

void Ccmd::notifyStreamEnded(std::string eventid) {
    CConfig::log("Main stream loop stopped", eventid);
    _stopEvent(eventid);

};

void Ccmd::notifyMakeMainImageStarted(std::string eventid) {
    CConfig::log("Main image loop Started", eventid);
}

void Ccmd::notifyMakeMainImageEnded(std::string eventid) {
    CConfig::log("Main image loop stopped", eventid);

    _stopEvent(eventid);
}


void Ccmd::startReadStream(std::string rtmpURL, int layerNumber) { // TODO: Delete!!!!

    /*   FFreader[layerNumber].dt.width = layerNumber;
       FFreader[layerNumber].dt.layer = layerNumber;
       std::cout << "start input " << rtmpURL << " " << layerNumber << std::endl;
       std::cout << "start  " << (FFreader[layerNumber].dt.width) << " " << layerNumber << std::endl;
       // worker.work(rtmpURL);

       std::thread ffmpegThread(FFreader[layerNumber].work, rtmpURL, &FFreader[layerNumber].dt, &locker);
       ffmpegThread.detach();
   */

    // ffmpegThread

}

void Ccmd::loadPresImage(std::string filepath, const std::string simageid) {

    // auto start = std::chrono::system_clock::now();

    clearPresImage();

    // std::lock_guard<std::mutex> lockGuard(locker);
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
    // std::lock_guard<std::mutex> lockGuard(locker);
    PresImageWidth = 0;
    PresImageHeight = 0;
    imageid = "";
    // free(PresImagePixels);
    PresImagePixels;
    // PresImagePixels=nullptr;

};

int Ccmd::stopEvent(const std::string eventid) {
    _stopEvent(eventid);
    return 0;
}

int Ccmd::startEvent(const std::string eventid) {

    if (Ccmd::_Events.find(eventid) != _Events.end()) {
        CConfig::log("start Event:: already started", eventid);
        return 0;
    }
    CConfig::log("startEvent", eventid);

    CEvent *event = new CEvent(eventid);

    Ccmd::_Events.insert({eventid, event});

    int w = CConfig::WIDTH;
    int h = CConfig::HEIGHT;
    event->mainImageData = SOIL_load_image("/etc/mixerCore/images/pgmbg.png",
                                           &w,
                                           &h,
                                           0,
                                           SOIL_LOAD_RGB);


    std::thread streamThread(CffmpegStreamer::startStream, eventid, event,
                             (std::function<void(std::string)>) notifyStreamStarted,
                             (std::function<void(std::string)>) notifyStreamEnded);
    streamThread.detach();
    event->thread = &streamThread;

    std::thread makeMainImageThread(Ccmd::makeMainImage,
                                    eventid,
                                    event,
                                    (std::function<void(std::string eventid)>) notifyMakeMainImageStarted,
                                    (std::function<void(std::string eventid)>) notifyMakeMainImageEnded);


    makeMainImageThread.detach();
    return 0;
}

bool Ccmd::showPres(std::string fileName, std::string eventid, std::string itemid) {


    std::lock_guard<std::mutex> lock(_locker);
    if (_Events.find(eventid) == _Events.end())
        return false;

    CEvent *event = _Events.at(eventid);
    Magick::Image image;
    image.read(fileName);
    unsigned char *buf = (unsigned char *) malloc(CConfig::WIDTH * CConfig::HEIGHT * 3 * sizeof(unsigned char));
    image.write(0, 0, CConfig::WIDTH, CConfig::HEIGHT, "RGB", MagickLib::CharPixel, buf);
    event->showPres(buf, itemid);
    free(buf);
    CConfig::log("show pres id:", itemid, ", event id:", eventid);
    return true;
};

bool Ccmd::activateInput(std::string eventid, int itemid) {

    std::lock_guard<std::mutex> lock(_locker);
    if (_Events.find(eventid) == _Events.end())
        return false;
    if (itemid >= CConfig::MAX_FACES)
        return false;
    auto event = _Events.at(eventid);
    event->locker.lock();
    event->activeInputs.clear();
    event->activeInputs.push_back(itemid);
    event->locker.unlock();
    CConfig::log("activate Input,", itemid, ", event id:", eventid);
    return true;
}