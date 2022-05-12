//
// Created by Denis Shevchenko on 12.05.2022.
//

#ifndef MIXERCORE_SSTREAMDATA_H
#define MIXERCORE_SSTREAMDATA_H

#include <thread>

struct SstreamData {
    std::string eventid;
    std::thread *thread;
};
typedef  std::map<std::string, SstreamData*> streamersDataType;


#endif //MIXERCORE_SSTREAMDATA_H
