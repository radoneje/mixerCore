
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

int main() {

    std::thread httpThread(httpServer.Init(8090));

    std::thread renderThread(render.StartRender);
    std::cout << "Hello, World!" << std::endl;
    renderThread.join();
    return 0;
}
