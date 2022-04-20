
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CRender.h"


CRender render;
int main() {
    std::thread thr(render.StartRender);
    std::cout << "Hello, World!" << std::endl;
    render.StartRender;
    return 0;
}
