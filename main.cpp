
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CRender.h"
#include "CHttp.h"
#include "Ccmd.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

///
#include <GL/glut.h>



CHttp httpServer;
CRender render;

int main(int argc, char* argv[]) {

    Ccmd *cmd= new Ccmd();
    std::thread httpThread(httpServer.init,8090, cmd);
    std::thread renderThread(render.StartRender, argc,  argv);
    std::cout << "Hello, World!" << std::endl;
    renderThread.join();
    std::cin.get();
    delete cmd;
    return 0;
}
