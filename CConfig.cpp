//
// Created by Denis Shevchenko on 25.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CConfig.h"
#include <libconfig.h++>

using namespace libconfig;

CConfig::CConfig() {};

int CConfig::GetConfig(const char *name, const char** str) {

    Config cfg;

   // if(! config_read_file(&cfg, "/etc/MixerCore/mixerCore.conf"))

}

