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
    width=0;
    height=0;
    numChannels=0;

}
