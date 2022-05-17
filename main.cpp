
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <map>



#include "CHttp.h"
#include "Ccmd.h"
#include "CffmpegStreamer.h"

#include "CConfig.h"



CHttp httpServer;
//CRender render;

int main(int argc, char* argv[]) {
    std::cout<< "!!!!! 55"<< std::endl;

    if(CConfig::getGlobalValues()>0)
        exit(1);
    Ccmd *cmd= new Ccmd();
    std::thread httpThread(httpServer.init,CConfig::HTTP_SERVER_PORT, cmd);
    httpThread.join();
    std::cin.get();
    return 0;

}
