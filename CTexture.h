//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CTEXTURE_H
#define MIXERCORE_CTEXTURE_H

#include <GL/glut.h>

/*class CTexture {

    struct sTextureImage
    {
        unsigned char* pixels;
        int width;
        int height;
        int numChannels;
    };

    class Texture
    {
    public:
        GLuint textureId;
        sTextureImage image;

        Texture();
        void Prepare (int textureId);
        void ReadPPMImage (char *fileName);

    };

};*/
class CTexture {
public:
    GLuint texName;
    Image image;
    void StartRender();
    CTexture();
    struct sTextureImage {
        unsigned char *pixels;
        int width;
        int height;
        int numChannels;
    };
    void Prepare(int textureId);

};


#endif //MIXERCORE_CTEXTURE_H
