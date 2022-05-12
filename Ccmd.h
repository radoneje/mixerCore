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
        static std::map<std::string, std::thread::id> streamers;
        int startStream(const std::string eventid);
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
        typedef void (* vFunctionCall)(std::string args);
        void startReadStream(std::string rtmpURL, int layerNumber);
        static void notifyStreamStarted(std::string eventid);
        static void notifyStreamEnded(std::string eventid);
        unsigned char* mainImageData;

        Ccmd();
};


#endif //MIXERCORE_CCMD_H
