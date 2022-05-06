//
// Created by Denis Shevchenko on 28.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

#include "Ccmd.h"

Ccmd::Ccmd(){

};

void Ccmd::loadPresImage(std::string filepath){
 std::cout<<"loadPresImage "  <<filepath <<std::endl;
}
void Ccmd::clearPresImage(){
    std::lock_guard<std::mutex> lockGuard(locker);
    PresImageWidth=0;
    PresImageHeight=0;
};