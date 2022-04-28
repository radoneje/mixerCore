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
#include "SOIL.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

///
#include <GL/glut.h>

CTextureData  CRender::textureData2;



//sImage CRender::texturePlaceholder;
std::vector<sImage> CRender::texturePlaceholder;
std::vector<std::vector<float>>  CRender::texturePosition(MAX_FACES, std::vector<float>(2));
GLuint CRender::textures[MAX_FACES+1];
sImage CRender::backgroundImage;
//GLuint CRender::textures;

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
    float fMinX = -1.0, fMinY = -1.0, fNearZ = 1.0,
            fMaxX = 1.0, fMaxY = 1.0, fFarZ = 10.0;

    { ///prepare
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

        for(int i=0; i<texturePlaceholder.size(); i++) {

           // glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, textures[i]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



        }
        glBindTexture(GL_TEXTURE_2D, textures[MAX_FACES]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    }
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(fMinX, fMaxX, fMinY, fMaxY, fNearZ, fFarZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void CRender::Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int cell=0, row=0;
    for(int i=0; i<MAX_FACES; i++) {

        if(cell>=4) {
            cell = 0;
            row++;

        }

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, textures[0]);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturePlaceholder[i].width,
                     texturePlaceholder[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texturePlaceholder[i].pixels);


        glBegin(GL_QUADS);
        if(row==0) {
            glTexCoord2f(0, 0);
            glColor3d(1, 0, 0);
            glVertex3f(-1 + (cell * 0.5), 1, -8);

            glTexCoord2f(0, 1);
            glColor3d(1, 1, 0);
            glVertex3f(-1 + (cell * 0.5), 1-0.5, -8);

            glTexCoord2f(1, 1);
            glColor3d(1, 1, 1);
            glVertex3f(-1 + 0.5 + (cell * 0.5), 1-0.5, -8);

            glTexCoord2f(1, 0);
            glColor3d(0, 1, 1);
            glVertex3f(-1 + 0.5 + (cell * 0.5), 1, -8);
        }
        else
        {
            glTexCoord2f(0, 0);
            glVertex3f(0.5, 1-0.5-(0.5*cell), -8);

            glTexCoord2f(0, 1);
            glVertex3f(0.5, 0.5-0.5-(0.5*cell), -8);

            glTexCoord2f(1, 1);
            glVertex3f(1, 0.5-0.5-(0.5*cell), -8);

            glTexCoord2f(1, 0);
            glVertex3f(1, 1-0.5-(0.5*cell), -8);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);



        cell++;
    }

    ///// make background

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textures[MAX_FACES+1]);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturePlaceholder[0].height,
                 texturePlaceholder[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texturePlaceholder[0].pixels);


    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-1, 1, -8);
        glTexCoord2f(0, 1);
        glVertex3f(-1, -1, -8);
        glTexCoord2f(1, 1);
        glVertex3f(1, -1, -8);
        glTexCoord2f(1, 0);
        glVertex3f(1, 1, -8);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
    glFlush();

}
    void CRender::StartRender(int argc, char **argv){


        for(int i=0; i<MAX_FACES; i++) {

            sImage item;

            std::string fileName;
            fileName.append("/etc/mixerCore/images/notconnected");
            fileName.append(std::to_string(i+1));
            fileName.append(".png");
            std::cout<<fileName<<std::endl;
            item.pixels =SOIL_load_image(fileName.c_str(),
                                                  &item.width,
                                                  &item.height,
                                                  0,
                                                  SOIL_LOAD_RGB);


            texturePlaceholder.push_back(item);

        }
        backgroundImage.pixels =SOIL_load_image("/etc/mixerCore/images/pgmbg.png",
                                                &backgroundImage.width,
                                                &backgroundImage.height,
                                                0,
                                                SOIL_LOAD_RGB);



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


            glGenTextures(MAX_FACES+1, textures);




        glClearColor(0.0, 0.0, 1.0, 0.0);
        glutDisplayFunc(Display);
        glutReshapeFunc(Reshape);
        glutIdleFunc(Display);     // used in animation

        // enter GLUT event processing cycle
        glutMainLoop();

        return;
    }


