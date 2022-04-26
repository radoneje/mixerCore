//
// Created by Denis Shevchenko on 26.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CTextureData.h"

CTextureData::CTextureData(){
    std::cout<< "CTextureData()"<< std::endl;
    this->width=0;
    this->height=0;
    this->numChannels=0;

}
