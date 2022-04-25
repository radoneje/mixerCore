//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H


struct sTextureImage {
public:
    unsigned char *pixels;
    int width;
    int height;
    int numChannels;

};
class CRender {
private:
    static void Reshape(int width, int height);
    static void  Display();

    void ReadPPMImage (char *fileName, sTextureImage &textureImage );
    static sTextureImage *textureImage= new sTextureImage[8];
    static void ReadPPMImage (char *fileName);
public:
    static void StartRender(int argc, char **argv);

    CRender();

};


#endif //MIXERCORE_CRENDER_H
