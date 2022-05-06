//
// Created by Denis Shevchenko on 06.05.2022.
//

#ifndef MIXERCORE_CFFREADER_H
#define MIXERCORE_CFFREADER_H


class CFFreader {

public:
    struct Data { ;
        unsigned char *pixels;
        int width;
        int height;
        int numChannels;
        int linesize;
        long frameNumber;
    };
public:
    static  int tmp;
    CFFreader();
    Data dt;
    static void work(const std::string url, Data  *pData , std::mutex *pLocker);
};


#endif //MIXERCORE_CFFREADER_H
