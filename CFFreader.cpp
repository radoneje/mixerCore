//
// Created by Denis Shevchenko on 06.05.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

#include "CFFreader.h"

CFFreader::CFFreader(){};
void CFFreader::work(std::string url){
    std::cout<< "CFFreader start work "<< url <<std::endl;
}

