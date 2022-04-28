//
// Created by Denis Shevchenko on 20.04.2022.
//

#include "CRender.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include "CConfig.h"
#include "CTextureData.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

///
#include <GL/glut.h>

  CTextureData * CRender::textureData[MAX_FACES];
CTextureData  CRender::textureData2;



sImage CRender::texturePlaceholder;

void CRender::ReadPPMImage2( const char* fileName, sImage *image) {

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

    sscanf (str,"%d %d",&image->width, &image->height);
    fgets (str,100,inFile);
    sscanf (str,"%d",&tmpint);

    if (tmpint != 255)
        printf("Warning: maxvalue is not 255 in ppm file\n");

    image->numChannels = 3;
    image->pixels = (unsigned char*) malloc (image->numChannels * image->width *  image->height * sizeof (unsigned char));

    if (image->pixels == NULL)
    {
        printf ("Can't allocate image of size %dx%d. Exiting\n", image->width, image->height);
        exit (1);
    }
    else
        printf("Reading image %s of size %dx%d\n", fileName, image->width, image->height);


    fread (image->pixels, sizeof (unsigned char), image->numChannels * image->width * image->height, inFile);

    fclose (inFile);

}

    CRender::CRender(){







    }
void CRender::Reshape(int width, int height){

    std::cout << "reshape" << width << " " << height << std::endl;
    float fMinX = -5.0, fMinY = -5.0, fNearZ = 1.0,
            fMaxX = 5.0, fMaxY = 5.0, fFarZ = 10.0;

    { ///prepare
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

     /*   glBindTexture (GL_TEXTURE_2D, 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturePlaceholder.width,
                     texturePlaceholder.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texturePlaceholder.pixels);*/
    }
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(fMinX, fMaxX, fMinY, fMaxY, fNearZ, fFarZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void CRender::Display(){

    glBindTexture(GL_TEXTURE_2D, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);



    char buf[1024];
    snprintf(buf, sizeof(buf), "/var/www/video-broadcast.space/102.ppm");
    ReadPPMImage2(buf, &texturePlaceholder);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturePlaceholder.width,
                 texturePlaceholder.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texturePlaceholder.pixels);
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-5, 5, -8);
    glTexCoord2f(0, 1);
    glVertex3f(-5, -5, -8);
    glTexCoord2f(1, 1);
    glVertex3f(5, -5, -8);
    glTexCoord2f(1, 0);
    glVertex3f(5, 5, -8);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
    glFlush();

}
    void CRender::StartRender(int argc, char **argv){

        for(int i=0; i<MAX_FACES; i++) {
            CTextureData tmpData;
            textureData[i]=&tmpData;
        }

       // std::cout<<CRender::textureData[0]->width<<std::endl;

        std::cout << "start render" << std::endl;
        std::string sWinName, sWinW, sWinH;


        if( CConfig::GetConfig("window",  sWinName)!=0) {
            std::cout <<  "ERROR: could read config file, varible window" << sWinName << '\n';
            return ;
        }
        if( CConfig::GetConfig("windowW",  sWinW)!=0) {
            std::cout <<  "ERROR: could read config file, varible windowW" << sWinW << '\n';
            return ;
        }
        if( CConfig::GetConfig("windowH",  sWinH)!=0) {
            std::cout <<  "ERROR: could read config file, varible windowH" << sWinH << '\n';
            return ;
        }
        //
        std::cout <<  std::stoi(sWinH.c_str()) << " <<-window name "<< '\n';

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowPosition(-1, -1);
        glutInitWindowSize(1280, 720);
        glutCreateWindow("OpenGL - Rotating Cubes");

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);

        GLuint t=1;
        glGenTextures(1, &t);

        char buf[1024];
        snprintf(buf, sizeof(buf), "/var/www/video-broadcast.space/102.ppm");
        ReadPPMImage2(buf, &texturePlaceholder);



        // register callbacks
        glutDisplayFunc(Display);
        glutReshapeFunc(Reshape);
        glutIdleFunc(Display);     // used in animation

        // enter GLUT event processing cycle
        glutMainLoop();

        return;
    }


