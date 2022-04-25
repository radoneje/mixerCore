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


CConfig::CConfig() {};

int CConfig::GetConfig(const char *name, char& str) {
    config_t cfg;
    config_setting_t *setting;


    config_init(&cfg);
    if(! config_read_file(&cfg, "/etc/MixerCore/mixerCore.conf"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return(EXIT_FAILURE);
    }
    if(config_lookup_string(&cfg, name, str))
        printf("Store name: %s\n\n", str);
    else
        fprintf(stderr, "No 'name' setting in configuration file.\n");

    config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
    return(EXIT_SUCCESS);
}

