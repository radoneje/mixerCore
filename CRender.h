//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H

#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

///
#include <GL/glut.h>
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
    float xRight;
    float yTop;
    float xLeft;
    float yBottom;
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
    static const int MAX_FACES=7;
   // static GLuint textures;
    static GLuint textures[MAX_FACES];
    static  CTextureData textureData2;
    static void ReadPPMImage2 (  const char * fileName, sImage *image  );
    static  std::vector<sImage> texturePlaceholder;
    static std::vector<std::vector<float>>  texturePosition;

   //static std::vector<CTextureData> textureImage;

public:

    static void StartRender(int argc, char **argv);
    CRender();

};


#endif //MIXERCORE_CRENDER_H
