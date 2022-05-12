//
// Created by Denis Shevchenko on 28.04.2022.
//

#ifndef MIXERCORE_CCMD_H
#define MIXERCORE_CCMD_H
#include <vector>
#include <map>
#include <mutex>
#include <thread>

#include "CFFreader.h"
#include "CffmpegStreamer.h"
#include "SstreamData.h"

using namespace std;

class Ccmd {
    private:
        std::map<std::string, SstreamData*> *_pStreamers;
    public:

        int startStream(const std::string eventid, std::map<std::string, SstreamData*> *pStreamers);

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
      //  typedef void (* vFunctionCall)(std::string args);
        void startReadStream(std::string rtmpURL, int layerNumber);
        static void notifyStreamStarted(std::string eventid, std::map<std::string, SstreamData*> *pStreamers);
        static void notifyStreamEnded(std::string eventid, std::map<std::string, SstreamData*> *pStreamers);
        unsigned char* mainImageData;

        Ccmd();
};


#endif //MIXERCORE_CCMD_H
