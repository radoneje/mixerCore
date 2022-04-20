
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
    std::thread thr(render.);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
