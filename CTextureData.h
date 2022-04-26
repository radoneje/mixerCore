//
// Created by Denis Shevchenko on 26.04.2022.
//

#ifndef MIXERCORE_CTEXTUREDATA_H
#define MIXERCORE_CTEXTUREDATA_H


class CTextureData {
public:
    static unsigned char *pixels;
    static int width;
    static int height;
    static int numChannels;

    static int test() { return 1; }

    CTextureData();

};


#endif //MIXERCORE_CTEXTUREDATA_H
