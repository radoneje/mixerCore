
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
#include "SstreamData.h"




CHttp httpServer;
//CRender render;

int main(int argc, char* argv[]) {


    Ccmd *cmd= new Ccmd();
    printf("startStream 0 \n");
    //std::map<std::string, SstreamData *> streamers;
    cmd->startStream("==event ID==");
    std::cin.get();




    return 0;
    std::thread httpThread(httpServer.init,8090, cmd);
  //  std::thread renderThread(render.StartRender, argc,  argv, cmd);
    std::cout << "Hello, World!" << std::endl;
   // renderThread.join();
    std::cin.get();
    delete cmd;
    return 0;
}
