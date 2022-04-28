//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H

#include <vector>
#include "CTextureData.h"

/*class sTextureImage {
public:
    unsigned char *pixels;
    int width;
    int height;
    int numChannels;
    sTextureImage(){
        width=0;
    };
    sTextureImage(int w, int h){
        width=w;
        height=0;
    };

};*/
struct sImage
{
    unsigned char* pixels;
    int width;
    int height;
    int numChannels;
};

class CRender {
private:
    /*struct sTextureImage{
    public:
        unsigned char *pixels;
        int width;
        int height;
        int numChannels;
    };*/
    static void Reshape(int width, int height);
    static void  Display();
    static const int MAX_FACES=8;
    static  CTextureData * textureData[MAX_FACES];

    static  CTextureData textureData2;
    static void ReadPPMImage2 (  const char * fileName, sImage *image  );
    static  std::vector<sImage> texturePlaceholder;

   //static std::vector<CTextureData> textureImage;

public:

    static void StartRender(int argc, char **argv);
    CRender();

};


#endif //MIXERCORE_CRENDER_H
