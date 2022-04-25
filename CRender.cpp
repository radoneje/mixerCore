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
#include "CConfig.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

///
#include <GL/glut.h>

void sTextureImage::ReadPPMImage(char *fileName) {
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

    sscanf (str,"%d %d",&this->width, &this->height);
    fgets (str,100,inFile);
    sscanf (str,"%d",&tmpint);

    if (tmpint != 255)
        printf("Warning: maxvalue is not 255 in ppm file\n");

    this->numChannels = 3;
    this->pixels = (unsigned char*) malloc (this->numChannels * this->width *  this->height * sizeof (unsigned char));

    if (this->pixels == NULL)
    {
        printf ("Can't allocate image of size %dx%d. Exiting\n", this->width, this->height);
        exit (1);
    }
    else
        printf("Reading image %s of size %dx%d\n", fileName, this->width, this->height);


    fread (this->pixels, sizeof (unsigned char), this->numChannels * this->width * this->height, inFile);

    fclose (inFile);
}

    CRender::CRender(){

    }
void CRender::Reshape(int width, int height){

    const float fMinX = -5.0, fMinY = -5.0, fNearZ = 1.0,
            fMaxX = 5.0, fMaxY = 5.0, fFarZ = 10.0;
    std::cout << "reshape" << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fMinX, fMaxX, fMinY, fMaxY, fNearZ, fFarZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void CRender::Display(){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0; i<8; i++){
        std::cout <<  textureImage[i].width << std::endl;
        continue;
        glBindTexture(GL_TEXTURE_2D, i+1);
        char buf[1024];
        snprintf(buf, sizeof(buf), "/var/www/video-broadcast.space/102.ppm");
        textureImage[i].ReadPPMImage(buf);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage[i].width,
                     textureImage[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     textureImage[i].pixels);

    }


    glutSwapBuffers();
    glFlush();

}
    void CRender::StartRender(int argc, char **argv){
        char argv1[0];
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
        glutInitWindowSize(std::stoi(sWinW), std::stoi(sWinH));
        glutCreateWindow(sWinName.c_str());
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);

        // register callbacks
        glutDisplayFunc(Display);
        glutReshapeFunc(Reshape);
        glutIdleFunc(Display);     // used in animation

        // enter GLUT event processing cycle
        glutMainLoop();

        return;
    }


