//
// Created by Denis Shevchenko on 06.05.2022.
//

#ifndef MIXERCORE_CFFREADER_H
#define MIXERCORE_CFFREADER_H


class CFFreader {

public:
    unsigned char* pixels;
    int width=0;
    int height=0;
    int numChannels=0;
    int tmp=1;
    CFFreader();
    int work(std::string url);
};


#endif //MIXERCORE_CFFREADER_H
