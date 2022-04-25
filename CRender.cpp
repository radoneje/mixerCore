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



    CRender::CRender(){}
void CRender::Reshape(int width, int height){

}
void CRender::Display(){

}
    void CRender::StartRender(int argc, char* argv[]){
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

        glutInit( 0, &argv);
       // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
      //  glutInitWindowPosition(-1, -1);
       // glutInitWindowSize(std::stoi(sWinW), std::stoi(sWinH));
       // glutCreateWindow(sWinName.c_str());
       // glClearColor(0.0, 0.0, 0.0, 0.0);
       // glEnable(GL_DEPTH_TEST);

        // register callbacks
      //  glutDisplayFunc(Display);
      //  glutReshapeFunc(Reshape);
      //  glutIdleFunc(Display);     // used in animation

        // enter GLUT event processing cycle
       // glutMainLoop();

        return;
    }


