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
        unsigned  char* loadNotConnected(int input);
    public:



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
        void startReadStream(std::string rtmpURL, int layerNumber); // TODO: Delete!!!!

        int startStream(const std::string eventid, std::map<std::string, SstreamData*> *pStreamers);
        static void notifyStreamStarted(std::string eventid, streamersDataType *pStreamers);
        static void notifyStreamEnded(std::string eventid, streamersDataType *pStreamers);

        std::map<std::string ,unsigned char*> mainImageData; // главная картинка // TODO:: сделать мфссив
        std::vector<unsigned char*> previewImageData;
        Ccmd();
};


#endif //MIXERCORE_CCMD_H
