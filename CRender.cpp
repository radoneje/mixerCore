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

void CRender::ReadPPMImage( const char* fileName, CTextureData *textureDataParam) {
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

    sscanf (str,"%d %d",(*textureDataParam).width, (*textureDataParam).height);
    fgets (str,100,inFile);
    sscanf (str,"%d",&tmpint);

    if (tmpint != 255)
        printf("Warning: maxvalue is not 255 in ppm file\n");

    (*textureDataParam).numChannels= 3;
    (*textureDataParam).pixels= (unsigned char*) malloc ((*textureDataParam).numChannels * (*textureDataParam).width *  (*textureDataParam).height * sizeof (unsigned char));

    if ((*textureDataParam).pixels == NULL)
    {
        printf ("Can't allocate image of size %dx%d. Exiting\n", (*textureDataParam).width, (*textureDataParam).height);
        exit (1);
    }
    else
        printf("Reading image %s of size %dx%d\n", fileName, (*textureDataParam).width, (*textureDataParam).height);


    fread ((*textureDataParam).pixels, sizeof (unsigned char), (*textureDataParam).numChannels * (*textureDataParam).width * (*textureDataParam).height, inFile);

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

    for(int i=0; i<MAX_FACES; i++){
        glBindTexture(GL_TEXTURE_2D, i+1);
        char buf[1024];
        snprintf(buf, sizeof(buf), "/var/www/video-broadcast.space/102.ppm");
        std::string filename;
        filename="/var/www/video-broadcast.space/102.ppm";
        ReadPPMImage( "/var/www/video-broadcast.space/102.ppm" , textureData[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData[i]->width,
                     textureData[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     textureData[i]->pixels);

    }


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


