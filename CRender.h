//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H

class sTextureImage {
public:
    unsigned char *pixels;
    int width;
    int height;
    int numChannels;

    sTextureImage(){
        this->width=0;
        this->height=0;
    };
    void ReadPPMImage (char *fileName);
};

class CRender {
private:
    static void Reshape(int width, int height);
    static void  Display();

    static sTextureImage textureImage[8];
    static void ReadPPMImage (char *fileName);
public:
    static void StartRender(int argc, char **argv);

    CRender();

};


#endif //MIXERCORE_CRENDER_H
