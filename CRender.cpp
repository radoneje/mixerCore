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



    CRender::CRender(){}
    void CRender::StartRender(){
        std::cout << "start render" << std::endl;
        std::string str;
        const char *name="window";
        if( CConfig::GetConfig(name,  *str)!=0) {
            std:std::cout <<  "ERROR: could read config file, varible" << name << '\n';
        }
        std::cout <<  str << " <<-window name "<< '\n';
      /*  glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowPosition(-1, -1);
        glutInitWindowSize(1280, 720);
        glutCreateWindow("OpenGL - Rotating Cubes");*/

        return;
    }


