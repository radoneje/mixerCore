//
// Created by Denis Shevchenko on 28.04.2022.
//

#ifndef MIXERCORE_CCMD_H
#define MIXERCORE_CCMD_H
#include <vector>
#include <map>
#include <mutex>

#include "CFFreader.h"
#include "CffmpegStreamer.h"

using namespace std;

class Ccmd {

    public:
        std::map<std::string, CffmpegStreamer*> streamers;
        int startStream(const std::string eventid, unsigned char * data);
        std::vector<CFFreader>  FFreader;
        std::vector<int> activeTextureId;
        std::mutex locker;
        void loadPresImage(std::string filepath, const std::string simageid);
        void clearPresImage();
        unsigned char* PresImagePixels;
        int PresImageWidth;
        int PresImageHeight;
        int PresImageNumChannels;
        std::string imageid;
        CFFreader meReader;
        typedef void (* vFunctionCall)(int args);
        void startReadStream(std::string rtmpURL, int layerNumber, vFunctionCall  notifyStreamStarted, vFunctionCall notifyStreamEnded);
        static void notifyStreamStarted(int layerNumber);
        static void notifyStreamEnded(int layerNumber);
        unsigned char* mainImageData;

        Ccmd();
};


#endif //MIXERCORE_CCMD_H
