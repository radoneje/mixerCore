//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CTEXTURE_H
#define MIXERCORE_CTEXTURE_H

class CTexture {
public:

    void StartRender();
    CTexture();
    struct sTextureImage {
        unsigned char *pixels;
        int width;
        int height;
        int numChannels;
    };
    void Prepare(int textureId);
    void ReadPPMImage (char *fileName);
    GLuint texName;
    sTextureImage TextureImage;

};


#endif //MIXERCORE_CTEXTURE_H
