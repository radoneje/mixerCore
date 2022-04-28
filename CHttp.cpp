//
// Created by Denis Shevchenko on 23.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "CHttp.h"
#include "httplib.h"

CHttp::CHttp(){


};
void CHttp::init(int port, Ccmd *pCmd){
    std::cout<< "http CHttp: "<< port <<std::endl;
    httplib::Server svr;
    svr.Get(R"(/mixer/activeInput/(\d+))", [&](const httplib::Request &req, httplib::Response &res) {
       // res.set_content("Hello World!", "text/plain");
        std::string value = req.matches[1];
        {
            std::lock_guard<std::mutex> lockGuard(pCmd->locker);
            pCmd->activeTextureId.clear();
            pCmd->activeTextureId.push_back(std::stoi(value));
        }

        res.set_content(value, "text/plain");
    });

    svr.listen("0.0.0.0", port);
}
