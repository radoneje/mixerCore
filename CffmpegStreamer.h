//
// Created by Denis Shevchenko on 10.05.2022.
//

#ifndef MIXERCORE_CFFMPEGSTREAMER_H
#define MIXERCORE_CFFMPEGSTREAMER_H


class CffmpegStreamer {
public:
    CffmpegStreamer();
    static void startStream(const std::string eventid);
    int init();
};


#endif //MIXERCORE_CFFMPEGSTREAMER_H
