//
// Created by Denis Shevchenko on 10.05.2022.
//

#ifndef MIXERCORE_CFFMPEGSTREAMER_H
#define MIXERCORE_CFFMPEGSTREAMER_H
#include <functional>


class CffmpegStreamer {
public:
    CffmpegStreamer();
    static void startStream( std::string eventid, unsigned char * image,  std::function<void(int)>* startCallback,   std::function<void(int)>* EndCallback);
    int init();
};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
