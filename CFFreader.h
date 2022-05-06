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
    };
public:
    static  int tmp;
    CFFreader();
    Data dt;
    static int work(std::string url, Data  &pData );
};


#endif //MIXERCORE_CFFREADER_H
