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

        int startStream(const std::string eventid);
        static void notifyStreamStarted(std::string eventid);
        static void notifyStreamEnded(std::string eventid);

        unsigned char * mainImageData; // главная картинка
        std::vector<unsigned char*> previewImageData;
static  void makeMainImage  (unsigned char * mainImageData, std::vector<unsigned char*> *previewImageData);//, std::function<void(std::string eventid)> onStart, std::function<void(std::string eventid)> onEnd );
static void notifyMakeMainImageStarted(std::string eventid);
static void notifyMakeMainImageEnded(std::string eventid);
        Ccmd();
};


#endif //MIXERCORE_CCMD_H
