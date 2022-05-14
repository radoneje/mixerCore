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
#include <cstdio>

#include "CHttp.h"
#include "httplib.h"
#include "CConfig.h"

CHttp::CHttp(){


};
void CHttp::init(int port, Ccmd *pCmd){
    CConfig::log( "HTTP Server listening port: ", port);
    httplib::Server svr;
    svr.Get(R"(/mixer/startEvent/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))", [&](const httplib::Request &req, httplib::Response &res) {
        // res.set_content("Hello World!", "text/plain");
        std::string eventid = req.matches[1];
        {
            pCmd->  clearPresImage();
            //std::lock_guard<std::mutex> lockGuard(pCmd->locker);
            pCmd->startEvent(eventid);
            res.set_content("{\"error\":false}", "application/json");
            // pCmd->locker.lock();
        }

    });
    svr.Get(R"(/mixer/stopEvent/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))", [&](const httplib::Request &req, httplib::Response &res) {
        // res.set_content("Hello World!", "text/plain");
        std::string eventid = req.matches[1];
        {
            pCmd->  clearPresImage();
            //std::lock_guard<std::mutex> lockGuard(pCmd->locker);
            pCmd->stopEvent(eventid);
            res.set_content("{\"error\":false}", "application/json");
            // pCmd->locker.lock();
        }

    });

    svr.Get("/mixer/startInput",[&](const httplib::Request &req, httplib::Response &res){
        res.set_content("{\"err\":false}", "application/json");
        pCmd->startReadStream(
                req.get_param_value("url"),
                std::stoi( req.get_param_value("id"))/*,
                (Ccmd::vFunctionCall)Ccmd::notifyStreamStarted,
                (Ccmd::vFunctionCall)Ccmd::notifyStreamEnded*/
                );
    });
    svr.Post(R"(/mixer/activatePresImg/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",[&](const httplib::Request &req, httplib::Response &res){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        auto start = std::chrono::system_clock::now();
        std::cout<< "/mixer/activatePresImg" << std::endl;
        const std::string eventid = req.matches[1];
        const std::string imageid = req.matches[2];
        if(imageid==pCmd->imageid)
        {
            pCmd->clearPresImage();
            std::string jsonResponce("{\"error\":false, \"presFileId\":\"\"}");
            res.set_content(jsonResponce, "application/json");
            return ;

        }
        auto ret = req.has_file("image");
        const auto& file = req.get_file_value("image");
        std::cout<< file.filename << " " << eventid << std::endl;

        std::ofstream myfile;
        std::string fileName("/tmp/pres"+eventid+".png");
        std::cout<< fileName << std::endl;
        myfile.open (fileName);
        myfile << file.content;
        myfile.close();


        pCmd->loadPresImage(fileName, imageid);

        std::remove(fileName.c_str());
        int timeDiff=( std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count())/1000;
        std::cout << "Time difference = " <<timeDiff << "[ms]" << std::endl;
        std::string jsonResponce("{\"error\":false, \"presFileId\":\""+imageid+"\"}");
        res.set_content(jsonResponce, "application/json");

    });
    svr.Get(R"(/mixer/activeInput/(\d+))", [&](const httplib::Request &req, httplib::Response &res) {
       // res.set_content("Hello World!", "text/plain");
        std::string value = req.matches[1];
        {
            pCmd->  clearPresImage();
          //  std::lock_guard<std::mutex> lockGuard(pCmd->locker);

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
