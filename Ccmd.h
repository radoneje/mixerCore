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

#include "CEvent.h"





using namespace std;

class Ccmd {
    private:
   
        static std::map<std::string, CEvent*> _Events;
        unsigned  char* loadNotConnected(int input);
        static void _stopEvent(std::string eventid);
        static std::mutex _locker;
    public:

        std::vector<CFFreader>  FFreader;
        std::vector<int> activeTextureId;
        std::string getEventStatus(std::string eventid);
        void loadPresImage(std::string filepath, const std::string simageid);
        void clearPresImage();
        unsigned char* PresImagePixels;
        int PresImageWidth;
        int PresImageHeight;
        int PresImageNumChannels;
        std::string imageid;
        CFFreader meReader;
      //  typedef void (* vFunctionCall)(std::string args);
        void startReadStream(std::string rtmpURL,std::string eventid, int layerNumber, std::string spkid); // TODO: Delete!!!!

        int startEvent(const std::string eventid);
        int stopEvent(const std::string eventid);
        static void notifyStreamStarted(std::string eventid);
        static void notifyStreamEnded(std::string eventid);

       // unsigned char * mainImageData; // главная картинка
      //  std::vector<unsigned char*> previewImageData;
        static  void makeMainImage  (std::string eventid, CEvent *pEvent, std::function<void(std::string eventid)> onStart, std::function<void(std::string eventid)> onEnd );
        static void notifyMakeMainImageStarted(std::string eventid);
        static void notifyMakeMainImageEnded(std::string eventid);
        Ccmd();


        bool showPres(std::string fileName, std::string eventid, std::string itemid);
        bool activateInput(std::string eventid,  int itemid);
};


#endif //MIXERCORE_CCMD_H
