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
    void CRender::StartRender(){
        std::cout << "start render" << std::endl;
        std::string sWinName, sWinW, sWinH;


        if( CConfig::GetConfig("widow",  sWinName)!=0) {
            std::cout <<  "ERROR: could read config file, varible" << sWinName << '\n';
        }
        if( CConfig::GetConfig("windowW",  sWinName)!=0) {
            std::cout <<  "ERROR: could read config file, varible" << sWinW << '\n';
        }
        if( CConfig::GetConfig("windowH",  sWinH)!=0) {
            std::cout <<  "ERROR: could read config file, varible" << sWinH << '\n';
        }
        std::cout <<  std::stoi(sWinW.c_str()) << " <<-window name "<< '\n';

        glutInit(NULL, NULL);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowPosition(-1, -1);
        glutInitWindowSize(std::stoi(sWinW), std::stoi(sWinH));
        glutCreateWindow(sWinName);

        return;
    }


