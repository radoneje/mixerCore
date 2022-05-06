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
#include <fstream>

#include "CHttp.h"
#include "httplib.h"

CHttp::CHttp(){


};
void CHttp::init(int port, Ccmd *pCmd){
    std::cout<< "http CHttp: "<< port <<std::endl;
    httplib::Server svr;
    svr.Post("/mixer/activatePresImg",[&](const httplib::Request &req, httplib::Response &res){
       std::cout<< "/mixer/activatePresImg" << std::endl;
        auto ret = req.has_file("image");
        const auto& file = req.get_file_value("image");
        const auto eventid =req.get_param_value("eventid");
        std::cout<< file.filename << " " << eventid << std::endl;

        std::ofstream myfile;
        std::string fileName("/tmp/pres"+eventid+".png");
        std::cout<< fileName << std::endl;
        myfile.open (fileName);
        myfile << file.content;
        myfile.close();

        // file.filename;
        // file.content_type;
        // file.content;

        res.set_content("{\"error\":false}", "application/json");
    });
    svr.Get(R"(/mixer/activeInput/(\d+))", [&](const httplib::Request &req, httplib::Response &res) {
       // res.set_content("Hello World!", "text/plain");
        std::string value = req.matches[1];
        {
            std::lock_guard<std::mutex> lockGuard(pCmd->locker);
           // pCmd->locker.lock();
            bool find=false;
            for(int i=0; i<pCmd->activeTextureId.size();i++){
                if(pCmd->activeTextureId[i]==std::stoi(value))
                    find=true;
            }
            pCmd->activeTextureId.clear();
            std::cout<<"activeInput: "<< value << find << std::endl;
            if(!find) {
                pCmd->activeTextureId.push_back(std::stoi(value));
                res.set_content(value, "application/json");
            }
            else
                res.set_content("-1", "application/json");
           // pCmd->locker.unlock();
        }

    });

    svr.listen("0.0.0.0", port);
}
