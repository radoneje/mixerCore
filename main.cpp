
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CRender.h"
#include "CHttp.h"

CHttp httpServer;
CRender render;

int main(int argc, char* argv[]) {

    std::thread httpThread(httpServer.init,8090);

    std::thread renderThread(render.StartRender, argc,  &argv);
    std::cout << "Hello, World!" << std::endl;
    renderThread.join();
    std::cin.get();
    return 0;
}
