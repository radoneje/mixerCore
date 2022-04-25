//
// Created by Denis Shevchenko on 20.04.2022.
//

#include "CTexture.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <GL/glut.h>
#include <GL/gl.h>
#pragma warning (disable : 4996)


#pragma warning (disable : 4996)

CTexture::CTexture(){}
void CTexture::StartRender(){
    std::cout << "start render" << std::endl;
    return;
}




void   CTexture::Prepare(int textureId)
{
    textureId = textureId;
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    glBindTexture (GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage.width,
                 TextureImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 TextureImage.pixels);
    // gluBuild2DMipmaps(GL_TEXTURE_2D, 0, GL_RGB, image.width,
    //                 image.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
    //                 image.pixels)
}

void  CTexture::ReadPPMImage (char *fileName)
{
    int tmpint;
    char str[100];
    FILE* inFile = fopen (fileName,"rb");

    if (inFile == NULL)
    {
        printf ("Can't open input file %s. Exiting.\n",fileName);
        exit (1);
    }

    fscanf (inFile,"P%d\n", &tmpint);

    if (tmpint != 6)
    {
        printf ("Input file is not ppm. Exiting. %d \n", tmpint);
        exit (1);
    }

    // skip comments embedded in header

    fgets (str,100,inFile);
    while (str[0]=='#')
        fgets(str,100,inFile);

    // read image dimensions

    sscanf (str,"%d %d",&TextureImage.width, &TextureImage.height);
    fgets (str,100,inFile);
    sscanf (str,"%d",&tmpint);

    if (tmpint != 255)
        printf("Warning: maxvalue is not 255 in ppm file\n");

    TextureImage.numChannels = 3;
    TextureImage.pixels = (unsigned char*) malloc (TextureImage.numChannels * TextureImage.width *  TextureImage.height * sizeof (unsigned char));

    if (TextureImage.pixels == NULL)
    {
        printf ("Can't allocate image of size %dx%d. Exiting\n", TextureImage.width, TextureImage.height);
        exit (1);
    }
    else
        printf("Reading image %s of size %dx%d\n", fileName, TextureImage.width, TextureImage.height);


    fread (TextureImage.pixels, sizeof (unsigned char), TextureImage.numChannels * TextureImage.width * TextureImage.height, inFile);

    fclose (inFile);
}

