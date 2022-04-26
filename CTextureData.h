//
// Created by Denis Shevchenko on 26.04.2022.
//

#ifndef MIXERCORE_CTEXTUREDATA_H
#define MIXERCORE_CTEXTUREDATA_H


class CTextureData {
    public:
        unsigned char *pixels;
        int width;
        int height;
        int numChannels;
    CTextureData();

};


#endif //MIXERCORE_CTEXTUREDATA_H
