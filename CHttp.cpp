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

CHttp::CHttp() {


};

void CHttp::init(int port, Ccmd *pCmd) {
    CConfig::log("HTTP Server listening port: ", port);
    httplib::Server svr;

    svr.Get(R"(/mixer/eventStatus/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",
            [&](const httplib::Request &req, httplib::Response &res) {
                // res.set_content("Hello World!", "text/plain");
                
                if(req.matches.size()>0)
                {
                    //std::lock_guard<std::mutex> lockGuard(pCmd->locker);
                    res.set_content(pCmd->getEventStatus(req.matches[1]), "application/json");
                    // pCmd->locker.lock();
                }
                else
                    res.set_content(std::string("{\"error\":true}"),"application/json" );


            });
    svr.Get(R"(/mixer/startEvent/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",
            [&](const httplib::Request &req, httplib::Response &res) {
                // res.set_content("Hello World!", "text/plain");
                if(req.matches.size()>0) {
                    std::string eventid = req.matches[1];
                    {
                        pCmd->clearPresImage();
                        //std::lock_guard<std::mutex> lockGuard(pCmd->locker);
                        pCmd->startEvent(eventid);
                        res.set_content("{\"error\":false}", "application/json");
                        // pCmd->locker.lock();
                    }
                }
                else
                    res.set_content("{\"error\":true}", "application/json");


            });
    svr.Get(R"(/mixer/stopEvent/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",
            [&](const httplib::Request &req, httplib::Response &res) {
                // res.set_content("Hello World!", "text/plain");
                std::string eventid = req.matches[1];
                {
                    //std::lock_guard<std::mutex> lockGuard(pCmd->locker);
                    pCmd->stopEvent(eventid);
                    res.set_content("{\"error\":false}", "application/json");
                    // pCmd->locker.lock();
                }

            });

    svr.Get("/mixer/startInput", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_content("{\"err\":false}", "application/json");
    
       
        pCmd->startReadStream(
                req.get_param_value("url"),
                req.get_param_value("eventid"),
                std::stoi(req.get_param_value("id")),
                req.get_param_value("spkid")
                /*,
                (Ccmd::vFunctionCall)Ccmd::notifyStreamStarted,
                (Ccmd::vFunctionCall)Ccmd::notifyStreamEnded*/
        );

            res.set_content("{\"error\":false}", "application/json");


    });
    svr.Get(
            R"(/mixer/loadPresVideo)",
            [&](const httplib::Request &req, httplib::Response &res) {
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                auto start = std::chrono::system_clock::now();

                const std::string eventid = req.get_param_value("eventid");
                const std::string fileid = req.get_param_value("fileid");
                const std::string url = req.get_param_value("url");
            
                 res.set_content(pCmd->loadPresVideo(eventid, fileid,url ), "application/json");
              
               /* CConfig::log("/mixer/activatePresImg", eventid);

                if (req.has_file("image")) {
                    const auto &file = req.get_file_value("image");
                    std::ofstream myfile;
                    std::string fileName("/tmp/pres" + eventid + ".png");
                    myfile.open(fileName);
                    myfile << file.content;
                    myfile.close();

                    std::string jsonResponce;
                    if (pCmd->showPres(fileName, eventid, imageid))
                        jsonResponce = ("{\"error\":false, \"presFileId\":\"" + imageid + "\"}");
                    else
                        jsonResponce = ("{\"error\":true}");

                    res.set_content(jsonResponce, "application/json");

                } else {
                    std::string jsonResponce("{\"error\":true, \"presFileId\":\"" + imageid + "\"}");
                    res.set_content(jsonResponce, "application/json");
                    CConfig::error("file not find in request show pores, eventid:", eventid, "itemid", imageid);
                }*/
                return;


            });
    svr.Post(
            R"(/mixer/activatePresImg/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}))",
            [&](const httplib::Request &req, httplib::Response &res) {
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                auto start = std::chrono::system_clock::now();

                const std::string eventid = req.matches[1];
                const std::string imageid = req.matches[2];
                CConfig::log("/mixer/activatePresImg", eventid);

                if (req.has_file("image")) {
                    const auto &file = req.get_file_value("image");
                    std::ofstream myfile;
                    std::string fileName("/tmp/pres" + eventid + ".png");
                    myfile.open(fileName);
                    myfile << file.content;
                    myfile.close();

                    std::string jsonResponce;
                    if (pCmd->showPres(fileName, eventid, imageid))
                        jsonResponce = ("{\"error\":false, \"presFileId\":\"" + imageid + "\"}");
                    else
                        jsonResponce = ("{\"error\":true}");

                    res.set_content(jsonResponce, "application/json");

                } else {
                    std::string jsonResponce("{\"error\":true, \"presFileId\":\"" + imageid + "\"}");
                    res.set_content(jsonResponce, "application/json");
                    CConfig::error("file not find in request show pores, eventid:", eventid, "itemid", imageid);
                }
                return;


            });
    svr.Get(R"(/mixer/activeInput/([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})/(\d+))",
            [&](const httplib::Request &req, httplib::Response &res) {
                // res.set_content("Hello World!", "text/plain");
                const std::string eventid = req.matches[1];
                std::string buf = req.matches[2];
                int value = atoi(buf.c_str());
                if (pCmd->activateInput(eventid, value)) {
                    res.set_content(to_string(value), "application/json");
                } else
                    res.set_content("-1", "application/json");

            });

    svr.listen("0.0.0.0", port);
}
