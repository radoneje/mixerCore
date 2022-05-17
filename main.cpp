
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
    //printf("startStream 0 \n");
    //std::map<std::string, SstreamData *> streamers;
   // cmd->startEvent("15dcce20-eec2-4a95-b556-26404597d218");
    std::cin.get();




    return 0;
 /*   std::thread httpThread(httpServer.init,8090, cmd);
  //  std::thread renderThread(render.StartRender, argc,  argv, cmd);
    std::cout << "Hello, World!" << std::endl;
   // renderThread.join();
    std::cin.get();
    delete cmd;
    return 0;*/
}
